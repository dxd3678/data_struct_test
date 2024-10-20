#include <stdio.h>
#include "queue.h"

// �ṹ��֪ʶ�̵�
// 1. �ȶ���ṹ�����ͣ�Ȼ����ṹ�����
struct STU
{
	char* name;
	int age;
};
struct STU stu;

// 2.����ṹ�����͵�ͬʱ�������
struct STU1
{
	char* name;
	int age;
} stu1;

// 3.�����ṹ�嶨��ṹ������������ַ�����ڶ��ַ�������������,�����ַ�����ʡȥ�˽ṹ����������,��ֱ�Ӹ����ṹ����,���ֽṹ�����������ǽṹ�����Ͳ��ܸ���
struct
{
	char* name;
	int age;
} stu2;

// ʵ�� queue
// �ο����£�https://segmentfault.com/a/1190000019529307
// https://github.com/TaborKelly/queue-example/blob/master/simple_queue_example.c

struct __alarm_node__
{
	char *name;
	
	// ������е�������ڵ�
	TAILQ_ENTRY(__alarm_node__) send_nodes;
	TAILQ_ENTRY(__alarm_node__) recv_nodes;
};

typedef struct __alarm_node__ ALARM_NODE;

// �������ͷ��������, ����ͷ����
TAILQ_HEAD(ALARM_SEND, ALARM_NODE) alarm_send_queue;
TAILQ_HEAD(ALARM_RECV, ALARM_NODE) alarm_recv_queue;

int tailq_normal_insert(void)
{
	// ��ʼ�����У���ʼ������ͷ
	TAILQ_INIT(&alarm_send_queue);

	// �ڵ㶨��
	ALARM_NODE init_error = { 0 };
	init_error.name = "init_error";

	ALARM_NODE read_error = { 0 };
	read_error.name = "read_error";

	ALARM_NODE write_error = { 0 };
	write_error.name = "write_error";

	// ������ն��У�����β�������Ƚ��ȳ�����һ���Ƚ������������������
	TAILQ_INSERT_TAIL(&alarm_send_queue, &init_error, send_nodes);
	TAILQ_INSERT_TAIL(&alarm_send_queue, &read_error, send_nodes);
	TAILQ_INSERT_TAIL(&alarm_send_queue, &write_error, send_nodes);

	// ��������
	ALARM_NODE *tmp = NULL;
	TAILQ_FOREACH(tmp, &alarm_send_queue, send_nodes)
	{
		printf("%s \r\n", tmp->name);
	}

	// �������У�����û��ֱ�ӵ����Ľӿڡ���Ҫ�ֶ���������get��һ����Ȼ����remove
	tmp = NULL;
	tmp = TAILQ_FIRST(&alarm_send_queue);
	printf("%s \r\n", tmp->name);
	
	TAILQ_REMOVE(&alarm_send_queue, tmp, send_nodes);

	// �ٴα������һ�����ӳ�Ա
	tmp = TAILQ_FIRST(&alarm_send_queue);
	printf("%s \r\n", tmp->name);

	return 0;
}

int tailq_one_field_two_head_insert(void)
{
	// ��ʼ�����У���ʼ������ͷ
	TAILQ_INIT(&alarm_send_queue);
	TAILQ_INIT(&alarm_recv_queue);

	// �ڵ㶨��
	ALARM_NODE init_error = { 0 };
	init_error.name = "init_error";

	ALARM_NODE read_error = { 0 };
	read_error.name = "read_error";

	ALARM_NODE write_error = { 0 };
	write_error.name = "write_error";

	// ���뷢�Ͷ���
	TAILQ_INSERT_TAIL(&alarm_send_queue, &init_error, send_nodes);
	TAILQ_INSERT_TAIL(&alarm_send_queue, &read_error, send_nodes);
	TAILQ_INSERT_TAIL(&alarm_send_queue, &write_error, send_nodes);

	// ������ն���
	TAILQ_INSERT_TAIL(&alarm_recv_queue, &init_error, send_nodes);
	// TAILQ_INSERT_TAIL(&alarm_recv_queue, &read_error, send_nodes);
	TAILQ_INSERT_TAIL(&alarm_recv_queue, &write_error, send_nodes);

	// ���� send ����
	ALARM_NODE* tmp = NULL;
	TAILQ_FOREACH(tmp, &alarm_send_queue, send_nodes)
	{
		printf("alarm_send_queue %s \r\n", tmp->name);
	}

	// ���� recv ����
	TAILQ_FOREACH(tmp, &alarm_recv_queue, recv_nodes)
	{
		printf("alarm_recv_queue %s \r\n", tmp->name);
	}
	return 0;
}

int tailq_two_field_on_head_insert(void)
{
	// ��ʼ�����У���ʼ������ͷ
	TAILQ_INIT(&alarm_send_queue);

	// �ڵ㶨��
	ALARM_NODE init_error = { 0 };
	init_error.name = "init_error";

	ALARM_NODE read_error = { 0 };
	read_error.name = "read_error";

	ALARM_NODE write_error = { 0 };
	write_error.name = "write_error";

	// ���뷢�Ͷ���
	TAILQ_INSERT_TAIL(&alarm_send_queue, &init_error, send_nodes);
	TAILQ_INSERT_TAIL(&alarm_send_queue, &read_error, send_nodes);
	TAILQ_INSERT_TAIL(&alarm_send_queue, &write_error, send_nodes);

	// ������ն���
	TAILQ_INSERT_TAIL(&alarm_send_queue, &init_error, recv_nodes);
	// TAILQ_INSERT_TAIL(&alarm_recv_queue, &read_error, send_nodes);
	TAILQ_INSERT_TAIL(&alarm_send_queue, &write_error, recv_nodes);

	// ���� send ����
	ALARM_NODE* tmp = NULL;
	TAILQ_FOREACH(tmp, &alarm_send_queue, send_nodes)
	{
		printf("alarm_send_queue %s \r\n", tmp->name);
	}

	// ���� recv ����
	TAILQ_FOREACH(tmp, &alarm_recv_queue, recv_nodes)
	{
		printf("alarm_recv_queue %s \r\n", tmp->name);
	}
	return 0;
}

int tailq_two_field_two_head_insert(void)
{
	// ��ʼ�����У���ʼ������ͷ
	TAILQ_INIT(&alarm_send_queue);
	TAILQ_INIT(&alarm_recv_queue);

	// �ڵ㶨��
	ALARM_NODE init_error = { 0 };
	init_error.name = "init_error";

	ALARM_NODE read_error = { 0 };
	read_error.name = "read_error";

	ALARM_NODE write_error = { 0 };
	write_error.name = "write_error";

	// ���뷢�Ͷ���
	TAILQ_INSERT_TAIL(&alarm_send_queue, &init_error, send_nodes);
	TAILQ_INSERT_TAIL(&alarm_send_queue, &read_error, send_nodes);
	TAILQ_INSERT_TAIL(&alarm_send_queue, &write_error, send_nodes);

	// ������ն���
	TAILQ_INSERT_TAIL(&alarm_recv_queue, &init_error, recv_nodes);
	// TAILQ_INSERT_TAIL(&alarm_recv_queue, &read_error, send_nodes);
	TAILQ_INSERT_TAIL(&alarm_recv_queue, &write_error, recv_nodes);

	// ���� send ����
	ALARM_NODE* tmp = NULL;
	TAILQ_FOREACH(tmp, &alarm_send_queue, send_nodes)
	{
		printf("alarm_send_queue %s \r\n", tmp->name);
	}

	// ���� recv ����
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