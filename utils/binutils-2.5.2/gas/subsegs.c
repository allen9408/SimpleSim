/* subsegs.c - subsegments -
   Copyright (C) 1987, 1990, 1991, 1992, 1993, 1994
   Free Software Foundation, Inc.

   This file is part of GAS, the GNU Assembler.

   GAS is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   GAS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GAS; see the file COPYING.  If not, write to
   the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.  */

/*
 * Segments & sub-segments.
 */

#include "as.h"

#include "subsegs.h"
#include "obstack.h"

frchainS *frchain_root, *frchain_now;

#ifndef BFD_ASSEMBLER
#ifdef MANY_SEGMENTS
segment_info_type segment_info[SEG_MAXIMUM_ORDINAL];

#else
/* Commented in "subsegs.h". */
frchainS *data0_frchainP, *bss0_frchainP;

#endif /* MANY_SEGMENTS */
char *const seg_name[] =
{
  "absolute",
#ifdef MANY_SEGMENTS
  "e0", "e1", "e2", "e3", "e4", "e5", "e6", "e7", "e8", "e9",
#else
  "text",
  "data",
  "bss",
#endif /* MANY_SEGMENTS */
  "unknown",
  "ASSEMBLER-INTERNAL-LOGIC-ERROR!",
  "expr",
  "debug",
  "transfert vector preload",
  "transfert vector postload",
  "register",
  "",
};				/* Used by error reporters, dumpers etc. */
#else /* BFD_ASSEMBLER */

/* Gas segment information for bfd_abs_section_ptr and
   bfd_und_section_ptr.  */
static segment_info_type *abs_seg_info;
static segment_info_type *und_seg_info;

#endif /* BFD_ASSEMBLER */

static void subseg_set_rest PARAMS ((segT, subsegT));

void
subsegs_begin ()
{
  /* Check table(s) seg_name[], seg_N_TYPE[] is in correct order */
#if !defined (MANY_SEGMENTS) && !defined (BFD_ASSEMBLER)
  know (SEG_ABSOLUTE == 0);
  know (SEG_TEXT == 1);
  know (SEG_DATA == 2);
  know (SEG_BSS == 3);
  know (SEG_UNKNOWN == 4);
  know (SEG_GOOF == 5);
  know (SEG_EXPR == 6);
  know (SEG_DEBUG == 7);
  know (SEG_NTV == 8);
  know (SEG_PTV == 9);
  know (SEG_REGISTER == 10);
  know (SEG_MAXIMUM_ORDINAL == SEG_REGISTER);
#endif

  obstack_begin (&frags, 5000);
  frchain_root = NULL;
  frchain_now = NULL;		/* Warn new_subseg() that we are booting. */
  /* Fake up 1st frag.  It won't be used=> is ok if obstack...
     pads the end of it for alignment. */
  frag_now = (fragS *) obstack_alloc (&frags, SIZEOF_STRUCT_FRAG);
  memset (frag_now, 0, SIZEOF_STRUCT_FRAG);

#ifndef BFD_ASSEMBLER
  /* This 1st frag will not be in any frchain.
     We simply give subseg_new somewhere to scribble. */
  now_subseg = 42;		/* Lie for 1st call to subseg_new. */
#ifdef MANY_SEGMENTS
  {
    int i;
    for (i = SEG_E0; i < SEG_UNKNOWN; i++)
      {
	subseg_set (i, 0);
	segment_info[i].frchainP = frchain_now;
      }
  }
#else
  subseg_set (SEG_DATA, 0);	/* .data 0 */
  data0_frchainP = frchain_now;

  subseg_set (SEG_BSS, 0);
  bss0_frchainP = frchain_now;

#endif /* ! MANY_SEGMENTS */
#endif /* ! BFD_ASSEMBLER */

}

/*
 *			subseg_change()
 *
 * Change the subsegment we are in, BUT DO NOT MAKE A NEW FRAG for the
 * subsegment. If we are already in the correct subsegment, change nothing.
 * This is used eg as a worker for subseg_set [which does make a new frag_now]
 * and for changing segments after we have read the source. We construct eg
 * fixSs even after the source file is read, so we do have to keep the
 * segment context correct.
 */
void
subseg_change (seg, subseg)
     register segT seg;
     register int subseg;
{
  now_seg = seg;
  now_subseg = subseg;

#ifdef BFD_ASSEMBLER
  {
    segment_info_type *seginfo;
    seginfo = (segment_info_type *) bfd_get_section_userdata (stdoutput, seg);
    if (! seginfo)
      {
	seginfo = (segment_info_type *) xmalloc (sizeof (*seginfo));
	memset ((PTR) seginfo, 0, sizeof (*seginfo));
	seginfo->fix_root = NULL;
	seginfo->fix_tail = NULL;
	seginfo->bfd_section = seg;
	seginfo->sym = 0;
	if (seg == bfd_abs_section_ptr)
	  abs_seg_info = seginfo;
	else if (seg == bfd_und_section_ptr)
	  und_seg_info = seginfo;
	else
	  bfd_set_section_userdata (stdoutput, seg, (PTR) seginfo);
      }
  }
#else
#ifdef MANY_SEGMENTS
  seg_fix_rootP = &segment_info[seg].fix_root;
  seg_fix_tailP = &segment_info[seg].fix_tail;
#else
  if (seg == SEG_DATA)
    {
      seg_fix_rootP = &data_fix_root;
      seg_fix_tailP = &data_fix_tail;
    }
  else if (seg == SEG_TEXT)
    {
      seg_fix_rootP = &text_fix_root;
      seg_fix_tailP = &text_fix_tail;
    }
  else
    {
      know (seg == SEG_BSS);
      seg_fix_rootP = &bss_fix_root;
      seg_fix_tailP = &bss_fix_tail;
    }

#endif
#endif
}

static void
subseg_set_rest (seg, subseg)
     segT seg;
     subsegT subseg;
{
  long tmp;			/* JF for obstack alignment hacking */
  register frchainS *frcP;	/* crawl frchain chain */
  register frchainS **lastPP;	/* address of last pointer */
  frchainS *newP;		/* address of new frchain */
  register fragS *former_last_fragP;
  register fragS *new_fragP;

  if (frag_now)		/* If not bootstrapping. */
    {
      frag_now->fr_fix = (char*) obstack_next_free (&frags) - frag_now->fr_literal;
      frag_wane (frag_now);	/* Close off any frag in old subseg. */
    }
  /*
   * It would be nice to keep an obstack for each subsegment, if we swap
   * subsegments a lot. Hence we would have much fewer frag_wanes().
   */
  {
    obstack_finish (&frags);
    /*
     * If we don't do the above, the next object we put on obstack frags
     * will appear to start at the fr_literal of the current frag.
     * Also, above ensures that the next object will begin on a
     * address that is aligned correctly for the engine that runs
     * this program.
     */
  }
  subseg_change (seg, (int) subseg);
  /*
   * Attempt to find or make a frchain for that sub seg.
   * Crawl along chain of frchainSs, begins @ frchain_root.
   * If we need to make a frchainS, link it into correct
   * position of chain rooted in frchain_root.
   */
  for (frcP = *(lastPP = &frchain_root);
       frcP && frcP->frch_seg <= seg;
       frcP = *(lastPP = &frcP->frch_next))
    {
      if (frcP->frch_seg == seg
	  && frcP->frch_subseg >= subseg)
	{
	  break;
	}
    }
  /*
   * frcP:		Address of the 1st frchainS in correct segment with
   *		frch_subseg >= subseg.
   *		We want to either use this frchainS, or we want
   *		to insert a new frchainS just before it.
   *
   *		If frcP==NULL, then we are at the end of the chain
   *		of frchainS-s. A NULL frcP means we fell off the end
   *		of the chain looking for a
   *		frch_subseg >= subseg, so we
   *		must make a new frchainS.
   *
   *		If we ever maintain a pointer to
   *		the last frchainS in the chain, we change that pointer
   *		ONLY when frcP==NULL.
   *
   * lastPP:	Address of the pointer with value frcP;
   *		Never NULL.
   *		May point to frchain_root.
   *
   */
  if (!frcP
      || (frcP->frch_seg > seg
	  || frcP->frch_subseg > subseg))	/* Kinky logic only works with 2 segments. */
    {
      /*
       * This should be the only code that creates a frchainS.
       */
      newP = (frchainS *) obstack_alloc (&frags, sizeof (frchainS));
      memset (newP, 0, sizeof (frchainS));
      /* This begines on a good boundary because a obstack_done()
	 preceeded it.  It implies an obstack_done(), so we expect
	 the next object allocated to begin on a correct boundary. */
      *lastPP = newP;
      newP->frch_next = frcP;	/* perhaps NULL */
      (frcP = newP)->frch_subseg = subseg;
      newP->frch_seg = seg;
      newP->frch_last = NULL;
#ifdef BFD_ASSEMBLER
      newP->fix_root = NULL;
      newP->fix_tail = NULL;
#endif
    }
  /*
   * Here with frcP ->ing to the frchainS for subseg.
   */
  frchain_now = frcP;
  /*
   * Make a fresh frag for the subsegment.
   */
  /* We expect this to happen on a correct boundary since it was
     proceeded by a obstack_done(). */
  tmp = obstack_alignment_mask (&frags);	/* JF disable alignment */
  obstack_alignment_mask (&frags) = 0;
  frag_now = (fragS *) obstack_alloc (&frags, SIZEOF_STRUCT_FRAG);
  memset (frag_now, 0, SIZEOF_STRUCT_FRAG);
  obstack_alignment_mask (&frags) = tmp;
  /* But we want any more chars to come immediately after the
     structure we just made. */
  new_fragP = frag_now;
  new_fragP->fr_next = NULL;
  /*
   * Append new frag to current frchain.
   */
  former_last_fragP = frcP->frch_last;
  if (former_last_fragP)
    {
      know (former_last_fragP->fr_next == NULL);
      know (frchain_now->frch_root);
      former_last_fragP->fr_next = new_fragP;
    }
  else
    {
      frcP->frch_root = new_fragP;
    }
  frcP->frch_last = new_fragP;
}

/*
 *			subseg_set(segT, subsegT)
 *
 * If you attempt to change to the current subsegment, nothing happens.
 *
 * In:	segT, subsegT code for new subsegment.
 *	frag_now -> incomplete frag for current subsegment.
 *	If frag_now==NULL, then there is no old, incomplete frag, so
 *	the old frag is not closed off.
 *
 * Out:	now_subseg, now_seg updated.
 *	Frchain_now points to the (possibly new) struct frchain for this
 *	sub-segment.
 *	Frchain_root updated if needed.
 */

#ifndef BFD_ASSEMBLER

segT
subseg_new (segname, subseg)
     const char *segname;
     subsegT subseg;
{
  int i;

  for (i = 0; i < (int) SEG_MAXIMUM_ORDINAL; i++)
    {
      const char *s;

      s = segment_name ((segT) i);
      if (strcmp (segname, s) == 0
	  || (segname[0] == '.'
	      && strcmp (segname + 1, s) == 0))
	{
	  subseg_set ((segT) i, subseg);
	  return (segT) i;
	}
#ifdef obj_segment_name
      s = obj_segment_name ((segT) i);
      if (strcmp (segname, s) == 0
	  || (segname[0] == '.'
	      && strcmp (segname + 1, s) == 0))
	{
	  subseg_set ((segT) i, subseg);
	  return (segT) i;
	}
#endif
    }

#ifdef obj_add_segment
  {
    segT new_seg;
    new_seg = obj_add_segment (segname);
    subseg_set (new_seg, subseg);
    return new_seg;
  }
#else
  as_bad ("Attempt to switch to nonexistent segment \"%s\"", segname);
  return now_seg;
#endif
}

void
subseg_set (seg, subseg)	/* begin assembly for a new sub-segment */
     register segT seg;		/* SEG_DATA or SEG_TEXT */
     register subsegT subseg;
{
#ifndef MANY_SEGMENTS
  know (seg == SEG_DATA || seg == SEG_TEXT || seg == SEG_BSS);
#endif

  if (seg != now_seg || subseg != now_subseg)
    {				/* we just changed sub-segments */
      subseg_set_rest (seg, subseg);
    }
}

#else /* BFD_ASSEMBLER */

segT
subseg_get (segname, force_new)
     const char *segname;
     int force_new;
{
  segT secptr;
  segment_info_type *seginfo;
  const char *now_seg_name = (now_seg
			      ? bfd_get_section_name (stdoutput, now_seg)
			      : 0);

  if (!force_new
      && now_seg_name
      && (now_seg_name == segname
	  || !strcmp (now_seg_name, segname)))
    return now_seg;

  if (!force_new)
    secptr = bfd_make_section_old_way (stdoutput, segname);
  else
    secptr = bfd_make_section_anyway (stdoutput, segname);

  seginfo = seg_info (secptr);
  if (! seginfo)
    {
      /* Check whether output_section is set first because secptr may
         be bfd_abs_section_ptr.  */
      if (secptr->output_section != secptr)
	secptr->output_section = secptr;
      seginfo = (segment_info_type *) xmalloc (sizeof (*seginfo));
      memset ((PTR) seginfo, 0, sizeof (*seginfo));
      seginfo->fix_root = NULL;
      seginfo->fix_tail = NULL;
      seginfo->bfd_section = secptr;
      if (secptr == bfd_abs_section_ptr)
	abs_seg_info = seginfo;
      else if (secptr == bfd_und_section_ptr)
	und_seg_info = seginfo;
      else
	bfd_set_section_userdata (stdoutput, secptr, (PTR) seginfo);
      seginfo->frchainP = NULL;
      seginfo->lineno_list_head = seginfo->lineno_list_tail = NULL;
      seginfo->sym = NULL;
      seginfo->dot = NULL;
    }
  return secptr;
}

segT
subseg_new (segname, subseg)
     const char *segname;
     subsegT subseg;
{
  segT secptr;
  segment_info_type *seginfo;

  secptr = subseg_get (segname, 0);
  subseg_set_rest (secptr, subseg);
  seginfo = seg_info (secptr);
  if (! seginfo->frchainP)
    seginfo->frchainP = frchain_now;
  return secptr;
}

/* Like subseg_new, except a new section is always created, even if
   a section with that name already exists.  */
segT
subseg_force_new (segname, subseg)
     const char *segname;
     subsegT subseg;
{
  segT secptr;
  segment_info_type *seginfo;

  secptr = subseg_get (segname, 1);
  subseg_set_rest (secptr, subseg);
  seginfo = seg_info (secptr);
  if (! seginfo->frchainP)
    seginfo->frchainP = frchain_now;
  return secptr;
}

void
subseg_set (secptr, subseg)
     segT secptr;
     subsegT subseg;
{
  if (! (secptr == now_seg && subseg == now_subseg))
    subseg_set_rest (secptr, subseg);
}

#ifndef obj_sec_sym_ok_for_reloc
#define obj_sec_sym_ok_for_reloc(SEC)	0
#endif

/* Get the gas information we are storing for a section.  */

segment_info_type *
seg_info (sec)
     segT sec;
{
  if (sec == bfd_abs_section_ptr)
    return abs_seg_info;
  else if (sec == bfd_und_section_ptr)
    return und_seg_info;
  else
    return (segment_info_type *) bfd_get_section_userdata (stdoutput, sec);
}

symbolS *
section_symbol (sec)
     segT sec;
{
  segment_info_type *seginfo = seg_info (sec);
  symbolS *s;

  if (seginfo == 0)
    abort ();
  if (seginfo->sym)
    return seginfo->sym;
  s = symbol_find (sec->name);
  if (!s)
    {
#ifndef EMIT_SECTION_SYMBOLS
#define EMIT_SECTION_SYMBOLS 1
#endif

      if (! EMIT_SECTION_SYMBOLS
#ifdef BFD_ASSEMBLER
	  && symbol_table_frozen
#endif
	  )
	/* Here we know it won't be going into the symbol table.  */
	s = symbol_create (sec->name, sec, 0, &zero_address_frag);
      else
	s = symbol_new (sec->name, sec, 0, &zero_address_frag);
      S_CLEAR_EXTERNAL (s);

      /* Use the BFD section symbol, if possible.  */
      if (obj_sec_sym_ok_for_reloc (sec))
	s->bsym = sec->symbol;
    }
  seginfo->sym = s;
  return s;
}

#endif /* BFD_ASSEMBLER */

/* end of subsegs.c */
