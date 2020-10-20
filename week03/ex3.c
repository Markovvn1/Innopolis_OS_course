#include <stdio.h>
#include <stdlib.h>

struct LinkedList
{
	struct LinkedList* next_node;
	int value;
};

void print_list(struct LinkedList* node)
{
	struct LinkedList* t = node->next_node;
	while (t != NULL)
	{
		if (t != node->next_node) printf(" ");
		printf("%d", t->value);
		t = t->next_node;
	}
	printf("\n");
}

struct LinkedList* _create_node(int value, struct LinkedList* next_node)
{
	struct LinkedList* node = (struct LinkedList*)malloc(sizeof(struct LinkedList));
	node->next_node = next_node;
	node->value = value;
	return node;
}

// Insert element on position n
void insert_node(struct LinkedList* node, int n, int value)
{
	struct LinkedList* t = node;
	for (int i = 0; i < n; i++) t = t->next_node;
	t->next_node = _create_node(value, t->next_node);
}

// Delete element on position n
void delete_node(struct LinkedList* node, int n)
{
	struct LinkedList* t = node;
	for (int i = 0; i < n; i++) t = t->next_node;
	struct LinkedList* temp = t->next_node;
	t->next_node = temp->next_node;
	free(temp);
}

struct LinkedList* LinkedList_create()
{
	return _create_node(0, NULL);
}

void LinkedList_free(struct LinkedList* node)
{
	while (node != NULL)
	{
		struct LinkedList* temp = node->next_node;
		free(node);
		node = temp;
	}
}

int main()
{
	struct LinkedList* list = LinkedList_create();
	insert_node(list, 0, 5);
	insert_node(list, 1, 4);
	insert_node(list, 2, 3);
	insert_node(list, 3, 2);
	insert_node(list, 4, 1);
	print_list(list);
	insert_node(list, 3, 8);
	print_list(list);
	delete_node(list, 3);
	print_list(list);
	LinkedList_free(list);

	return 0;
}