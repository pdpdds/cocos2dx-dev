#ifndef _SOCKETS_SINGLETON_H
#define _SOCKETS_SINGLETON_H

#ifdef SOCKETS_NAMESPACE
namespace SOCKETS_NAMESPACE {
#endif

class Singleton
{
public:
	Singleton() {}
	~Singleton() {}

public:
	static Singleton&	GetInstance()
	{
		static Singleton m_bInstance;
		return m_bInstance;
	}

	char	szTemp[10000];
	char	szTemp2[10000];
	char	szTemp3[10000];

	int		a;
};



#ifdef SOCKETS_NAMESPACE
} // namespace SOCKETS_NAMESPACE {
#endif

#endif // _SOCKETS_Json_H
