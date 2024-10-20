#include <stdio.h>
#include "queue.h"

// 结构体知识铺垫
// 1. 先定义结构体类型，然后定义结构体变量
struct STU
{
	char* name;
	int age;
};
struct STU stu;

// 2.定义结构体类型的同时定义变量
struct STU1
{
	char* name;
	int age;
} stu1;

// 3.匿名结构体定义结构体变量，第三种方法与第二种方法的区别在于,第三种方法中省去了结构体类型名称,而直接给出结构变量,这种结构体最大的问题是结构体类型不能复用
struct
{
	char* name;
	int age;
} stu2;

// 实践 queue
// 参考文章：https://segmentfault.com/a/1190000019529307
// https://github.com/TaborKelly/queue-example/blob/master/simple_queue_example.c

struct __alarm_node__
{
	char *name;
	
	// 定义队列的作用域节点
	TAILQ_ENTRY(__alarm_node__) send_nodes;
	TAILQ_ENTRY(__alarm_node__) recv_nodes;
};

typedef struct __alarm_node__ ALARM_NODE;

// 定义队列头数据类型, 队列头变量
TAILQ_HEAD(ALARM_SEND, ALARM_NODE) alarm_send_queue;
TAILQ_HEAD(ALARM_RECV, ALARM_NODE) alarm_recv_queue;

int tailq_normal_insert(void)
{
	// 初始化队列：初始化队列头
	TAILQ_INIT(&alarm_send_queue);

	// 节点定义
	ALARM_NODE init_error = { 0 };
	init_error.name = "init_error";

	ALARM_NODE read_error = { 0 };
	read_error.name = "read_error";

	ALARM_NODE write_error = { 0 };
	write_error.name = "write_error";

	// 插入接收队列，插入尾部。（先进先出，第一个先进来，最后进来的在最后）
	TAILQ_INSERT_TAIL(&alarm_send_queue, &init_error, send_nodes);
	TAILQ_INSERT_TAIL(&alarm_send_queue, &read_error, send_nodes);
	TAILQ_INSERT_TAIL(&alarm_send_queue, &write_error, send_nodes);

	// 遍历队列
	ALARM_NODE *tmp = NULL;
	TAILQ_FOREACH(tmp, &alarm_send_queue, send_nodes)
	{
		printf("%s \r\n", tmp->name);
	}

	// 弹出队列，好像没有直接弹出的接口。需要手动操作：先get第一个，然后将其remove
	tmp = NULL;
	tmp = TAILQ_FIRST(&alarm_send_queue);
	printf("%s \r\n", tmp->name);
	
	TAILQ_REMOVE(&alarm_send_queue, tmp, send_nodes);

	// 再次遍历其第一个出队成员
	tmp = TAILQ_FIRST(&alarm_send_queue);
	printf("%s \r\n", tmp->name);

	return 0;
}

int tailq_one_field_two_head_insert(void)
{
	// 初始化队列：初始化队列头
	TAILQ_INIT(&alarm_send_queue);
	TAILQ_INIT(&alarm_recv_queue);

	// 节点定义
	ALARM_NODE init_error = { 0 };
	init_error.name = "init_error";

	ALARM_NODE read_error = { 0 };
	read_error.name = "read_error";

	ALARM_NODE write_error = { 0 };
	write_error.name = "write_error";

	// 插入发送队列
	TAILQ_INSERT_TAIL(&alarm_send_queue, &init_error, send_nodes);
	TAILQ_INSERT_TAIL(&alarm_send_queue, &read_error, send_nodes);
	TAILQ_INSERT_TAIL(&alarm_send_queue, &write_error, send_nodes);

	// 插入接收队列
	TAILQ_INSERT_TAIL(&alarm_recv_queue, &init_error, send_nodes);
	// TAILQ_INSERT_TAIL(&alarm_recv_queue, &read_error, send_nodes);
	TAILQ_INSERT_TAIL(&alarm_recv_queue, &write_error, send_nodes);

	// 遍历 send 队列
	ALARM_NODE* tmp = NULL;
	TAILQ_FOREACH(tmp, &alarm_send_queue, send_nodes)
	{
		printf("alarm_send_queue %s \r\n", tmp->name);
	}

	// 遍历 recv 队列
	TAILQ_FOREACH(tmp, &alarm_recv_queue, recv_nodes)
	{
		printf("alarm_recv_queue %s \r\n", tmp->name);
	}
	return 0;
}

int tailq_two_field_on_head_insert(void)
{
	// 初始化队列：初始化队列头
	TAILQ_INIT(&alarm_send_queue);

	// 节点定义
	ALARM_NODE init_error = { 0 };
	init_error.name = "init_error";

	ALARM_NODE read_error = { 0 };
	read_error.name = "read_error";

	ALARM_NODE write_error = { 0 };
	write_error.name = "write_error";

	// 插入发送队列
	TAILQ_INSERT_TAIL(&alarm_send_queue, &init_error, send_nodes);
	TAILQ_INSERT_TAIL(&alarm_send_queue, &read_error, send_nodes);
	TAILQ_INSERT_TAIL(&alarm_send_queue, &write_error, send_nodes);

	// 插入接收队列
	TAILQ_INSERT_TAIL(&alarm_send_queue, &init_error, recv_nodes);
	// TAILQ_INSERT_TAIL(&alarm_recv_queue, &read_error, send_nodes);
	TAILQ_INSERT_TAIL(&alarm_send_queue, &write_error, recv_nodes);

	// 遍历 send 队列
	ALARM_NODE* tmp = NULL;
	TAILQ_FOREACH(tmp, &alarm_send_queue, send_nodes)
	{
		printf("alarm_send_queue %s \r\n", tmp->name);
	}

	// 遍历 recv 队列
	TAILQ_FOREACH(tmp, &alarm_recv_queue, recv_nodes)
	{
		printf("alarm_recv_queue %s \r\n", tmp->name);
	}
	return 0;
}

int tailq_two_field_two_head_insert(void)
{
	// 初始化队列：初始化队列头
	TAILQ_INIT(&alarm_send_queue);
	TAILQ_INIT(&alarm_recv_queue);

	// 节点定义
	ALARM_NODE init_error = { 0 };
	init_error.name = "init_error";

	ALARM_NODE read_error = { 0 };
	read_error.name = "read_error";

	ALARM_NODE write_error = { 0 };
	write_error.name = "write_error";

	// 插入发送队列
	TAILQ_INSERT_TAIL(&alarm_send_queue, &init_error, send_nodes);
	TAILQ_INSERT_TAIL(&alarm_send_queue, &read_error, send_nodes);
	TAILQ_INSERT_TAIL(&alarm_send_queue, &write_error, send_nodes);

	// 插入接收队列
	TAILQ_INSERT_TAIL(&alarm_recv_queue, &init_error, recv_nodes);
	// TAILQ_INSERT_TAIL(&alarm_recv_queue, &read_error, send_nodes);
	TAILQ_INSERT_TAIL(&alarm_recv_queue, &write_error, recv_nodes);

	// 遍历 send 队列
	ALARM_NODE* tmp = NULL;
	TAILQ_FOREACH(tmp, &alarm_send_queue, send_nodes)
	{
		printf("alarm_send_queue %s \r\n", tmp->name);
	}

	// 遍历 recv 队列
	TAILQ_FOREACH(tmp, &alarm_recv_queue, recv_nodes)
	{
		printf("alarm_recv_queue %s \r\n", tmp->name);
	}
	return 0;
}

int main(void)
{
	tailq_normal_insert();
	return 0;
}