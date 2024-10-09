#include "sysc/kernel/sc_module.h"
#include "sysc/kernel/sc_time.h"
#include "systemc.h"
#include "tlm_utils/peq_with_get.h"
#include <sys/types.h>

// refer https://blog.csdn.net/zgcjaxj/article/details/104877270
// 关于 wait() 和 wait(SC_TIME_ZERO) https://blog.csdn.net/zgcjaxj/article/details/123171172
// peq_with_get 用于设置路径延迟，对Pipeline场景有用


using namespace sc_core;
using namespace std;


class TestPlatform : public sc_module {
    public:
        SC_CTOR(TestPlatform) : m_period(sc_time(1000, SC_PS)), m_test_peq("test_peq") {
            SC_THREAD(push_peq_1);
            SC_THREAD(push_peq_2);

            SC_THREAD(get_peq);

            sensitive << m_test_peq.get_event();
        }

    void push_peq_1() {
        uint * num1 = new uint;
        *num1 = 100;

        m_test_peq.notify(*num1, 10*m_period);
        cout<<"["<<sc_time_stamp()
		<<"] notify number 1 to peq, notify cycle = 10"
		<<endl;
    }

    void push_peq_2() {

        wait(2 * m_period);
        uint * num2 = new uint;
        *num2 = 20;

        m_test_peq.notify(*num2, 3*m_period);
        cout<<"["<<sc_time_stamp()
		<<"] notify number 2 to peq, notify cycle = 3"
		<<endl;
    }

    void get_peq() {
        uint * data = nullptr;
        while(true) {
            wait(); // 等待所有敏感事件

            // 这里如果使用if代替，无法获取同一时刻到达的多个trans
            while((data = m_test_peq.get_next_transaction())) {
                cout<<"["<<sc_time_stamp()
				<<"] get number "
				<< *data
				<<endl;

			delete data; //dynamic memory space, delete when no use
			data = nullptr;
            }
        }
    }

    public:
        sc_time m_period;
        tlm_utils::peq_with_get<uint> m_test_peq;
};


int sc_main(int argc, char* argv[]) {
    auto platform = new TestPlatform("Platform");
    sc_start(1, sc_core::SC_US);
    return 0;
}