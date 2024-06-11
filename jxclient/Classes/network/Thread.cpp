//#include "stdafx.h"
#include "Thread.h"
//
#ifdef WIN32
//#include "Macro.h"
#include <process.h>		//Thread define
CThread::CThread()
	: m_hThread( NULL )
{

}
CThread::~CThread()
{
	
#ifdef WIN32
	try{ if (m_hThread) {CloseHandle(m_hThread); (m_hThread) = NULL; } } catch(...) { }
#else
	SAFE_CLOSEHANDLE(m_hThread);
#endif
}

void * CThread::GetHandle() const
{
	return m_hThread;
}

void CThread::Start()
{
	if (m_hThread ==NULL)
	{
		unsigned int threadIDc = 0;
		//��ʼ�����߳�
		m_hThread = (void *)::_beginthreadex(0, 
			0, 
			ThreadFunction,
			(void *)this, 
			0, 
			&threadIDc);

		//��1����������ȫ���ԣ�NULLΪĬ�ϰ�ȫ����
		//��2��������ָ���̶߳�ջ�Ĵ�С�����Ϊ0�����̶߳�ջ��С�ʹ��������̵߳���ͬ��һ����0
		//��3��������ָ���̺߳����ĵ�ַ��Ҳ�����̵߳���ִ�еĺ�����ַ(�ú������Ƽ��ɣ��������ƾͱ�ʾ��ַ)
		//��4�����������ݸ��̵߳Ĳ�����ָ�룬����ͨ����������ָ�룬���̺߳�������ת��Ϊ��Ӧ���ָ��
		//��5���������̳߳�ʼ״̬��0:�������У�CREATE_SUSPEND��suspended�����ң�
        //��6�����������ڼ�¼�߳�ID�ĵ�ַ
		char ninfo[64]={0};
		if (m_hThread == NULL)
		{
			//sprintf(ninfo,"_beginthreadex:filed");
			//CCMessageBox(ninfo,"_beginthreadex");
			return;
		}

		//CCMessageBox("_beginthreadex:suss","_beginthreadex");
	}
}

void CThread::Wait() const
{
	if (!Wait(INFINITE))
	{
//		throw CException(_T("CThread::Wait()"), _T("Unexpected timeout on infinite wait"));
	}
}

bool CThread::Wait(unsigned long timeoutMillis) const
{
	bool ok;

	if ( !m_hThread )
	{
		return true;
	}

	DWORD result = ::WaitForSingleObject( m_hThread, timeoutMillis );

	if ( result == WAIT_TIMEOUT )
	{
		ok = false;
	}
	else if ( result == WAIT_OBJECT_0 )
	{
		ok = true;
	}
	else
	{
		//throw CWin32Exception( _T( "CThread::Wait() - WaitForSingleObject" ), ::GetLastError() );
	}
    
	return ok;
}
//�̻߳ص����� __stdcall
unsigned int __stdcall  CThread::ThreadFunction( void *pV )
{
	int result = 0;

	CThread* pThis = ( CThread * )pV;
   
	if (pThis)
	{
		try
		{
			//result = 
			pThis->Run();
		}
		catch(...)
		{
		}
	}

	return result;
}
//��ֹ�߳�
void CThread::Terminate(unsigned long exitCode /* = 0 */)
{
	if (m_hThread && !::TerminateThread(m_hThread,exitCode))
	{
		//TRACE( "CThread::Terminate error!" );
	}

	//SAFE_CLOSEHANDLE(m_hThread);
	try{ if (m_hThread) {CloseHandle(m_hThread); (m_hThread) = NULL; } } catch(...) { }
}
#else
//linux
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

CThread::CThread()
	:a_thread(0),
	autoDelete_(false)
{
}

CThread::~CThread()
{
	if (&thread_attr)
		pthread_attr_destroy(&thread_attr);
}

pthread_t CThread::GetHandle() const
{
	return a_thread;
}

void CThread::setAutoDelete(bool autoDelete)
{
	autoDelete_ = autoDelete;
}
//�̻߳ص����� __stdcall  
void * CThread::ThreadFunction(void *pV)
{//ִ�����߳�
	CThread* pThis = (CThread *)pV;

	if (pThis)
	{

		try
		{
			//result =
			//pthread_detach(pthread_self()); //���÷���״̬
			pThis->Run();
			//Terminate(NULL);
			if (pThis->autoDelete_)
			{
				delete pThis;
			}
		}
		catch(...)
		{
		}
	}

	//return;
}

void CThread::Start()
{
	/*int pthread_attr_getdetachstat(const pthread_attr_t *restrict attr,int *detachstate);
	  int pthread_attr_setdetachstat(const pthread_attr_t *attr,int detachstate);
       ����:�ɹ�����0,�����ش�����
	   detachstatȡֵ��
	   PTHREAD_CREATE_JOINABLE //(Ĭ��ֵ)���������߳�
	   PTHREAD_CREATE_DETACHED //�Է���״̬�����߳�
	   */
		pthread_attr_init(&thread_attr);
		pthread_attr_setdetachstate(&thread_attr,PTHREAD_CREATE_DETACHED);//���÷���״̬
		//pthread_create(&a_thread, &thread_attr, thread_function, NULL);
		//pthread_create(&a_thread, &thread_attr, thread_function, NULL);
		//��ʼ�������߳�
		//int err;
		//err = pthread_create(&a_thread,NULL,ThreadFunction,this);
		pthread_create(&a_thread, &thread_attr,ThreadFunction,this);
		//pthread_create(&thid2,NULL,(void*)thread2,NULL);
		//pthread_attr_destroy(&thread_attr);
		//return NULL;
}


void CThread::Terminate( unsigned long exitCode)
{
	//CCMessageBox("exit the thread","exit the thread");
	if (exitCode)
	   pthread_attr_destroy(&thread_attr);
	//if (a_thread)
	  //pthread_cancel(a_thread);

	//pthread_exit(NULL); //����һ���˳����ź�
	/*(1) ִ����ɺ���ʽ�˳���

		(2) ���̱߳�����ʾ����pthread_exit �����˳���
		pthread_exit (void * retval) ; 

	(3) �������߳���pthread_cancel������ֹ��
		pthread_cancel(pthread_t thread); */
	//pthread_exit((void *)0);
}


void CThread::Wait() const
{//�ȴ��߳̽���
	if (a_thread > 0)
	{//�ȴ��˳����ź�
		pthread_join(a_thread,NULL);
	}

}

void CThread::Wait(unsigned long timeoutMillis) const
{
	if (a_thread == 0)
	{
		return;
	}
	if (timeoutMillis == 0)
	{
		Wait();
	}else
	{
		unsigned long k = 0;
		while (/*threadStatus != THREAD_STATUS_EXIT && */k <= timeoutMillis)
		{
			usleep(100);
			k++;
		}
	}
}


#endif  //end linxu
