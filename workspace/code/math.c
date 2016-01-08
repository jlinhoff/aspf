// Copyright (C) 2016 Joe Linhoff - see license
#include "base.h"

////////////////////////////////////////////////////////////////////////////////
// base

int SPFInit()
{
   return 0;
} // SPFInit()

void SPFFinal()
{
} // SPFFinal

////////////////////////////////////////////////////////////////////////////////
// ListNode

// JFL 23 Aug 06
// JFL 20 Mar 08; re-worked from DL
// JFL 25 Oct 15
void ListMakeHead(ListNode *h)
{
   h->n=h->p=h;
   h->t=h->f=0;
} // ListMakeHead()

// JFL 20 Mar 08; re-worked from DL
// JFL 18 May 08; link to self
// JFL 25 Oct 15
void ListMakeNode(ListNode *n,int t)
{
   n->n=n->p=n;
   n->t=t,n->f=0;
} // ListMakeNode()

// JFL 23 Aug 06
// JFL 20 Mar 08; re-worked from DL
// JFL 08 Mar 10; fixed link bug
void ListLinkAfter(ListNode *h,ListNode *n)
{
   n->p=h;
   n->n=h->n;
   h->n->p=n;
   h->n=n;
} // ListLinkAfter()

// JFL 05 May 06
// JFL 20 Mar 08; re-worked from DL
void ListLinkBefore(ListNode *h,ListNode *n)
{
   n->p=h->p;
   n->p->n=n;
   n->n=h;
   h->p=n;
} // ListLinkBefore()

// JFL 05 May 06
// JFL 20 Mar 08; re-worked from DL
void ListUnlink(ListNode *n)
{
   n->p->n=n->n;
   n->n->p=n->p;
   n->n=n->p=n; // multiple unlinks OK
} // ListUnlink()

// EOF