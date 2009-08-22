#ifndef Swing_Lists_H
#define Swing_Lists_H

#include "SESystem.h"

//----------------------------------------------------------------------------
// ����:˫������ڵ���
// ˵��:�ο���Serious Engine
// ����:Sun Che
// ʱ��:20070603
//----------------------------------------------------------------------------
class SE_FOUNDATION_API ListNode 
{
public:
    ListNode* pSuccessor;
    ListNode* pPredecessor;

    bool IsValid(void) const;

public:
    inline ListNode(void);
    inline ListNode(const ListNode& rNode);
    inline ~ListNode(void);
  
public:
    inline ListNode &operator = (const ListNode& rNode);
    // ��鵱ǰ�ڵ��Ƿ�����list,�Լ�ָ��ǰ������,��ǰ�����̾�ָ���Լ�
    bool IsLinked(void) const;
    void AddAfter(ListNode& rNode);
    void AddBefore(ListNode& rNode);
    // ��list���Ƴ��Լ�
    void Remove(void);

    inline bool IsHeadMarker(void) const;
    inline bool IsTailMarker(void) const;
    inline bool IsHead(void) const;
    inline bool IsTail(void) const;

    // ��ȡ��ǰ�ڵ�ĺ�̽ڵ�
    inline ListNode& GetSucc(void) const;
    // ��ȡ��ǰ�ڵ��ǰ���ڵ�
    inline ListNode& GetPred(void) const;

    // ������
    inline ListNode& IterationSucc(void) const;
    // ��ǰ����
    inline ListNode& IterationPred(void) const;
    // �ڵ����������ڵ�ǰ�ڵ�֮ǰ����node
    inline void IterationInsertBefore(ListNode& rNode);
    // �ڵ����������ڵ�ǰ�ڵ�֮�����node
    inline void IterationInsertAfter(ListNode& rNode);

    // �ҵ���ǰ�ڵ�����list��list head
    ListHead& GetListHead(void);
};

//----------------------------------------------------------------------------
// ����:˫�������ͷ��
// ˵��:�ο���Serious Engine
// ����:Sun Che
// ʱ��:20070603
//----------------------------------------------------------------------------
class SE_FOUNDATION_API ListHead 
{
public:
    // ǰ2��ָ����ڴ�ռ��������list��head marker,
    // ��2��ָ����ڴ�ռ��������list��tail marker,
    // ��listʱ,head marker��pSuccessor(pHead)ָ��&pNULL,��ָ��tail marker,
    // ��listʱ,tail marker��pPredecessor(pTail)ָ��&pHead,��ָ��head marker,
    // head marker��pPredecessor(pNULL)ʼ��ΪNULL,tail marker��pSuccessor(pNULL)ʼ��ΪNULL.
    ListNode* pHead; // ָ���һ���ڵ�(head�ڵ�)
    ListNode* pNULL; // ����head marker��ǰ��ָ��,����tail marker�ĺ��ָ��
    ListNode* pTail; // ָ�����һ���ڵ�(tail�ڵ�)

     // �����ǿ�list,�����ǰ�������һ����Ч�ڵ��list
    bool IsValid(void) const;
    // ������List,
    // �������:0<-head marker-><-tail marker->0
    void Clear(void);

public:
    inline ListHead(void);
    inline ListHead(const ListHead& rHead);

public:
    inline void operator = (const ListHead& rHead);
  
    inline ListNode& GetHead(void) const;
    inline ListNode& GetTail(void) const;
    inline ListNode& GetIterationHead(void) const;
    inline ListNode& GetIterationTail(void) const;

    void AddHead(ListNode& rNode);
    void AddTail(ListNode& rNode);
    void RemoveHead(void);
    void RemoveTail(void);
    void RemoveAll(void);

    bool IsEmpty(void) const;
    // ����һ��list������Ԫ��move����
    void MoveList(ListHead& rHead);
    INDEX Count(void) const;
    void Sort(int (*pCompare)(const void *p0, const void *p1), int iNodeOffset);
};

//----------------------------------------------------------------------------
// ����:˫�������������
// ˵��:�ο���Serious Engine
// ����:Sun Che
// ʱ��:20070603
//----------------------------------------------------------------------------
template<class Base, int iOffset>
class ListIter 
{
public:
    ListIter(void){ m_pCurrentNode = 0; }
    // ָ��head(�ǿ�list)����tail marker(��list)
    ListIter(const ListHead& rHead){ m_pCurrentNode = &rHead.GetIterationHead(); }
    // ָ�����node
    ListIter(ListNode& lnNode){ ASSERT( rNode.IsLinked() ); m_pCurrentNode = &rNode; }

public:
    // ָ��head marker
    void Reset(const ListHead& rHead){ m_pCurrentNode = &rHead.IterationHead(); }
    // ���̵���
    void MoveToNext(void){ m_pCurrentNode = &m_pCurrentNode->IterationSucc(); }
    // ��ǰ������
    void MoveToPrev(void){ m_pCurrentNode = &m_pCurrentNode->IterationPred(); }
    // �Ƿ񵽴�tail marker
    bool IsPastEnd(void){ return m_pCurrentNode->IsTailMarker(); }

    // �ڵ�ǰָ��ڵ�֮�����ڵ�
    inline void InsertAfterCurrent(ListNode& rNode){ m_pCurrentNode->IterationInsertAfter(rNode); }
    // �ڵ�ǰָ��ڵ�֮ǰ����ڵ�
    inline void InsertBeforeCurrent(ListNode& rNode){ m_pCurrentNode->IterationInsertBefore(rNode); }

    // ��ȡ��ǰָ���Ԫ��
    Base& Current(void){ return *((Base*)((UBYTE*)m_pCurrentNode - iOffset)); }
    Base& operator* (void){ return *((Base*)((UBYTE*)m_pCurrentNode - iOffset)); }
    operator Base* (void){ return ((Base*)((UBYTE*)m_pCurrentNode - iOffset)); }
    Base* operator -> (void){ return ((Base*)((UBYTE*)m_pCurrentNode - iOffset)); }

private:
    ListNode* m_pCurrentNode;
};

#include "SELists.inl"

// offsetof���m��s�е��ֽ�ƫ����
//#define offsetof(s,m)	(size_t)&(((s *)0)->m)

// ��������baseclass�ĵ�����
#define LISTITER(baseclass, member) ListIter<baseclass, offsetof(baseclass, member)>

// �������list
#define FOREACHINLIST(baseclass, member, head, iter) \
  for( LISTITER(baseclass, member) iter(head); !iter.IsPastEnd(); iter.MoveToNext() )

// �������list
#define FOREACHINLIST_R(baseclass, member, head, iter) \
  for( LISTITER(baseclass, member) iter(head.IterationTail()); \
   !iter->member.IsHeadMarker(); iter.MoveToPrev() )

// �������ɾ��list
#define FORDELETELIST(baseclass, member, head, iter)		  \
   for( LISTITER(baseclass, member) iter(head), iter##next;	  \
   iter##next=iter, iter##next.IsPastEnd() || (iter##next.MoveToNext(),1), !iter.IsPastEnd(); \
     iter = iter##next)

// �������ɾ��list
#define FORDELETELIST_R(baseclass, member, head, iter)		  \
   for( LISTITER(baseclass, member) iter(head.IterationTail()), iter##prev;	  \
   iter##prev=iter, iter##prev->member.IsHeadMarker() || (iter##prev.MoveToPrev(),1), !iter->member.IsHeadMarker(); \
     iter = iter##prev)

#define LIST_HEAD(listhead, baseclass, member) \
  ( (baseclass *) ( ((UBYTE *)(&(listhead).GetHead())) - offsetof(baseclass, member) ) )

#define LIST_TAIL(listhead, baseclass, member) \
  ( (baseclass *) ( ((UBYTE *)(&(listhead).GetTail())) - offsetof(baseclass, member) ) )

#define LIST_PRED(element, baseclass, member) \
  ( (baseclass *) ( ((UBYTE *)(&(element).member.GetPred())) - offsetof(baseclass, member) ) )

#define LIST_SUCC(element, baseclass, member) \
  ( (baseclass *) ( ((UBYTE *)(&(element).member.GetSucc())) - offsetof(baseclass, member) ) )


#endif

