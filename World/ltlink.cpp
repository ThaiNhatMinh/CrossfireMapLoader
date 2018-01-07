#include "stdafx.h"
#include "ltlink.h"

void CheapLTLink::Init() {
	TieOff();
}

void CheapLTLink::Term() {
	Remove();
}

void CheapLTLink::Remove() {
	m_pPrev->m_pNext = m_pNext;
	m_pNext->m_pPrev = m_pPrev;
	TieOff();
}

void CheapLTLink::TieOff() {
	m_pPrev = m_pNext = (LTLink*)this;
}

void CheapLTLink::AddAfter(CheapLTLink *pLink) {
	pLink->m_pPrev = (LTLink*)this;
	pLink->m_pNext = m_pNext;
	pLink->m_pPrev->m_pNext = pLink->m_pNext->m_pPrev = (LTLink*)pLink;
}

void CheapLTLink::AddBefore(CheapLTLink *pLink) {
	pLink->m_pPrev = m_pPrev;
	pLink->m_pNext = (LTLink*)this;
	pLink->m_pPrev->m_pNext = pLink->m_pNext->m_pPrev = (LTLink*)pLink;
}

/*!
Make it easy to traverse the list in either direction.
*/
LTLink* CheapLTLink::operator[](unsigned long i) {
	return ((LTLink**)this)[i];
}

bool CheapLTLink::IsTiedOff() {
	return m_pPrev == this && m_pNext == this;
}
