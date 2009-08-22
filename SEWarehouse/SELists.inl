//----------------------------------------------------------------------------
inline ListNode::ListNode()
{
    pSuccessor = 0;
    pPredecessor = 0;
}
//----------------------------------------------------------------------------
inline ListNode::ListNode(const ListNode& rNode)
{
    (void)rNode;

    pSuccessor = 0;
    pPredecessor = 0;
}
//----------------------------------------------------------------------------
inline ListNode::~ListNode()
{
    if( IsLinked() ) 
    {
        Remove();
    }
}
//----------------------------------------------------------------------------
inline ListNode& ListNode::operator=(const ListNode& rNode)
{
    (void)rNode;

    pSuccessor = 0;
    pPredecessor = 0;
    
    return *this;
}
//----------------------------------------------------------------------------
inline ListNode& ListNode::GetSucc() const
{
    // �������ȡ��tail marker
    ASSERT( IsLinked() && pSuccessor->IsLinked() );

    return *pSuccessor;
}
//----------------------------------------------------------------------------
inline ListNode& ListNode::GetPred() const
{
    // �������ȡ��head marker
    ASSERT( IsLinked() && pPredecessor->IsLinked() );

    return *pPredecessor;
}
//----------------------------------------------------------------------------
inline ListNode& ListNode::IterationSucc() const
{
    // ���������tail marker
    ASSERT( pSuccessor->IsTailMarker() || pSuccessor->IsLinked() );
  
    return *pSuccessor;
}
//----------------------------------------------------------------------------
inline ListNode& ListNode::IterationPred() const
{
    // ���������head marker
    ASSERT( pPredecessor->IsHeadMarker() || pPredecessor->IsLinked() );
    
    return *pPredecessor;
}
//----------------------------------------------------------------------------
inline void ListNode::IterationInsertAfter(ListNode& rNode)
{
    ASSERT( !rNode.IsLinked() );
    ASSERT( pSuccessor->IsTailMarker() || pSuccessor->IsLinked() );
  
    pSuccessor->pPredecessor = &rNode;
    rNode.pSuccessor = pSuccessor;
    rNode.pPredecessor = this;
    pSuccessor = &rNode;
}
//----------------------------------------------------------------------------
inline void ListNode::IterationInsertBefore(ListNode& rNode)
{
    ASSERT( !rNode.IsLinked() );
    ASSERT( pPredecessor->IsHeadMarker() || pPredecessor->IsLinked() );
  
    pPredecessor->pSuccessor = &rNode;
    rNode.pPredecessor = pPredecessor;
    rNode.pSuccessor = this;
    pPredecessor = &rNode;
}
//----------------------------------------------------------------------------
inline bool ListNode::IsHeadMarker() const
{
    if( pPredecessor == 0 ) 
    {
        return true;
    } 
    else 
    {
        ASSERT( IsLinked() );
        return false;
    }
}
//----------------------------------------------------------------------------
inline bool ListNode::IsTailMarker() const
{
    if( pSuccessor == 0 ) 
    {
        return true;
    } 
    else 
    {
        ASSERT( IsLinked() );
        return false;
    }
}
//----------------------------------------------------------------------------
inline bool ListNode::IsHead() const
{
    ASSERT( IsLinked() );

    // head��ǰ����head marker
    return pPredecessor->pPredecessor == 0;
}
//----------------------------------------------------------------------------
inline bool ListNode::IsTail() const
{
    ASSERT( IsLinked() );

    // tail�ĺ����tail marker
    return pSuccessor->pSuccessor == 0;
}
//----------------------------------------------------------------------------
inline ListHead::ListHead() 
{ 
    Clear(); 
}
//----------------------------------------------------------------------------
inline ListHead::ListHead(const ListHead& rHead) 
{
    ASSERTALWAYS("Don't copy list heads!");
}
//----------------------------------------------------------------------------
inline void ListHead::operator=(const ListHead& rHead) 
{
    ASSERTALWAYS("Don't copy list heads!");
}
//----------------------------------------------------------------------------
inline ListNode& ListHead::GetHead() const
{
    ASSERT( IsValid() && pHead->IsLinked() );
  
    return *pHead;
}
//----------------------------------------------------------------------------
inline ListNode& ListHead::GetTail() const
{
    ASSERT( IsValid() && pTail->IsLinked() );
  
    return *pTail;
}
//----------------------------------------------------------------------------
inline ListNode& ListHead::GetIterationHead() const
{
    // ���������tail marker
    ASSERT( IsValid() && (pHead->IsTailMarker() || pHead->IsLinked()) );
  
    return *pHead;
}
//----------------------------------------------------------------------------
inline ListNode& ListHead::GetIterationTail() const
{
    // ���������head marker
    ASSERT( IsValid() && (pTail->IsHeadMarker() || pTail->IsLinked()) );
  
    return *pTail;
}
//----------------------------------------------------------------------------