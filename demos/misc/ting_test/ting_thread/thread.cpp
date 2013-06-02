
/// Tested with ting 1.0.0
/// based on: http://code.google.com/p/ting/wiki/SocketsUsage

#include <iostream>
#include <mt/Thread.hpp>
#include <mt/MsgThread.hpp>
#include <mt/Message.hpp>

/// Simple Ting::Thread
class MyThread : public ting::mt::Thread
{
public:
    /// Override Thread:Run()
    void Run()
    {
        /// do some thread actions
        std::cout<< "Thread 1: I'm Up and Running!" << std::endl;
    }
};

/// Ting Message/Queues Thread
class MyMsgThread : public ting::mt::MsgThread
{
public:
    /// Define some variables specific to your thread,
    /// In this example we have two ints: a and b
    int a;
    int b;

public:
    /// Override ThreadMsg:Run()
    void Run()
    {
        while(!this->quitFlag)
        {
            /// Handle all messages on the queue
            while(ting::Ptr<ting::mt::Message> m = this->queue.PeekMsg())
            {
                m->Handle();
                if( this->quitFlag )
                    break;
            }

            /// do some actions to be done every cycle
            std::cout<< "Thread 2: Cycle Done!" << std::endl;
        }
    }
};

/// Ting Message
class MyMessage : public ting::mt::Message
{
private:
    MyMsgThread *myThr;

public:
    MyMessage(MyMsgThread *mt) :
            myThr(mt)
    {
        /// assert that this->myThr is not 0.
        /// ...
    }

    /// Override
    void Handle()
    {
        /// Implement handler for this message
        this->myThr->a = 10;
        this->myThr->b = 20;

        std::cout<<"Message Handled!"<<std::endl;

        /// If you want to quit thread after processing this message:
        /// myThr->quitFlag = true;
    }
};

int main()
{
    /// create an object on the stack for simplicity
    MyThread thr;
    MyMsgThread msg_thr;

    msg_thr.Start();
    /// start thread execution
    thr.Start();
    /// Wait for thread to finish its execution.
    thr.Join();

    /// create the message
    ting::Ptr<ting::mt::Message> msg( new MyMessage(&msg_thr) );

    /// send the message
    msg_thr.PushMessage(msg);

    for(int i=0; i<10; ++i)
        std::cout<<"Main: Loop!"<<std::endl;

    /// Send quit message to msg_thr
    ting::Ptr<ting::mt::Message> quit( new ting::mt::QuitMessage(&msg_thr) );
    msg_thr.PushMessage(quit);
    /// Wait for msg_thr to be quited
    msg_thr.Join();

    std::cout<< "Main: Done!" << std::endl;
    return 0;
}

/*
 OUTPUT:

Thread 1: I'm Up and Running!Thread 2: Cycle Done!

Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Thread 2: Cycle Done!
Main: Loop!
Main: Loop!
Main: Loop!
Main: Loop!
Main: Loop!
Main: Loop!
Main: Loop!
Main: Loop!
Main: Loop!
Main: Loop!
Message Handled!
Thread 2: Cycle Done!
Main: Done!

 */
