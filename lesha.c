#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>

struct List {
	char symbol;
	struct List *next;
};

void del(struct List **current) {
	struct List *tmp;
	tmp = *current;
	*current = (*current)->next;
	free(tmp);
}

void destroy(struct List **head) {
	struct List *tmp;
	while (*head) {
		tmp = *head;
		*head = (*head)->next;
		free(tmp);
	}
}

struct List *skipSpace(struct List *current) {
	while ((current) && ((current->symbol == ' ') || (current->symbol == '\t'))) {
		del(&current);
	}
	return current;
}

struct List *skipWord(struct List *current) {
	while ((current) && (current->symbol != ' ') && (current->symbol != '\t')) {
		del(&current);
	}
	return current;
}

int getLengthWord(struct List *current) {
	int length = 0;
	while ((current->next) && (current->next->symbol != ' ') && (current->next->symbol != '\t')) {
		length++;
		current = current->next;
	}
	return length;
}

void changeTab(struct List *current) {
	if (current->symbol == '\t')
		current->symbol = ' ';
}

struct List  *make(struct List *head) {
	head = skipSpace(head);
	if (!head) return NULL;
	struct List *newHead = head;
	int lengthFirtsWord, lengthCurrentWord;
	lengthFirtsWord = getLengthWord(head);
	while (head->next) {
		if ((head->next->symbol == ' ') || (head->next->symbol == '\t')) {
			changeTab(head->next);
			head->next->next = skipSpace(head->next->next);
			if (!head->next->next) {
				del(&head->next);
				break;
			}
			lengthCurrentWord = getLengthWord(head->next->next);
			if (lengthCurrentWord != lengthFirtsWord)
				head->next = skipWord(head->next->next);
			else
				head = head->next;
		}
		else
			head = head->next;
	}
	return newHead;
}

int input(struct List **head) {
	printf("Input string:\n");
	char buf[256];
	int code;
	struct List newHead = { '*', NULL };
	struct List *last = &newHead;
	do {
		code = scanf("%255[^\n]", buf);
		if (code == 0)
			continue;
		if (code < 0) {
			return code;
		}
		int i = 0;
		while (buf[i] != '\0') {
			last->next = (struct List*)malloc(sizeof(struct List));
			last = last->next;
			last->symbol = buf[i++];
			last->next = NULL;
		}
	} while (code > 0);
	*head = newHead.next;
	scanf("%*c");
	return 1;
}

void out(struct List *head, const char *defaultText) {
	printf("%s\n|", defaultText);
	while (head) {
		printf("%c", head->symbol);
		head = head->next;
	}
	printf("|\n");
}
int main() {
	struct List *head = NULL;
	while (1) {
		int code = input(&head);
		if (code <= 0) {
			if (code)
				return 0;
			scanf("%*c");
			continue;
		}
		out(head, "Before:");
		head = make(head);
		out(head, "After:");
		destroy(&head);
		printf("\n");
	}
	return (0);
}
