#pragma once



typedef enum
{
	LTLink_Init = 0
} LTLinkCommand;


#if !defined(force_inline)
#if defined(_MSC_VER) && (_MSC_VER >= 1200) 
// MS Visual C++ version 6.0 and later
#define force_inline __forceinline
#else
#define force_inline inline
#endif
#define local_force_inline
#endif // !defined(force_inline)

/*!  Use \b LTLink whenever you need \b m_pData, but you can use this if you
have a list head.  */

class LTLink;
class CheapLTLink {
public:

	void Init();
	 void Term();
	 void Remove();
	 void TieOff();
	 void AddAfter(CheapLTLink *pLink);
	 void AddBefore(CheapLTLink *pLink);
	 LTLink *operator[](unsigned long i);
	 LTLink *AsDLink() { return (LTLink*)this; }
	 LTLink *AsLTLink() { return (LTLink*)this; }
	 bool IsTiedOff();


public:

	LTLink *m_pPrev, *m_pNext;
};


class LTLink : public CheapLTLink {
public:

	 LTLink() {}
	 LTLink(LTLinkCommand cmd) {
		TieOff();
	}

	 void Init2(void *pData) {
		TieOff();
		m_pData = pData;
	}


public:
	void *m_pData;
};

