
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>




struct n {
	int number;
	struct n* left;
	struct n* right;
	struct n* up_left;
	struct n*up_right;
	struct n* down_left;
	struct n* down_right;
};
typedef struct n n;    

void n_ini(n* a)    //node initial 
{
	a->number = 0;
	a->left = NULL;
	a->right = NULL;
	a->up_left = NULL;
	a->up_right = NULL;
	a->down_left = NULL;
	a->down_right = NULL;
}

int n_count(n*c_right)                //check how many nodes in that level
{
	int left = 0;
	int right = 0;
	n* c_left = c_right;   //copy
	while (c_left->left != NULL)
	{
		c_left = c_left->left;
		left++;
	}
	while (c_right->right != NULL)
	{
		c_right = c_right->right;
		right++;
	}
	return left + right + 1;
}


n* n_level_insert(n**head, n* current, int e)   //give level's first node pointer
{
	n* backup = NULL;
	n* temp = (n*)malloc(sizeof(n));
	n_ini(temp);
	while (1)
	{
		if (e <= current->number) ///insert at left side, also check those links 
		{
			if (current->left == NULL)  ///insert at left side as first node of current level, need to fix pointer 
			{
				current->left = temp;
				temp->right = current;
				temp->number = e;
				temp->up_left = current->up_left;
				temp->up_right = current->up_right;
				if ((current->up_left == NULL) && (current->up_right == NULL))
					*head = temp; //headnode link correction
				else
				{
					if (current->up_left != NULL)  //up level node link correction
						current->up_left->down_right = temp;
					if (current->up_right != NULL)
						current->up_right->down_left = temp;
				}
				break;
			}
			else
			{
				backup = current->left;
				current->left = temp;
				backup->right = temp;
				temp->right = current;
				temp->left = backup;
				temp->number = e;
				temp->up_left = current->up_left;
				temp->up_right = current->up_right;
				break;
			}
		}
		else if (current->right != NULL)
		{
			current = current->right;
		}
		else                     ///insert at right side
		{
			current->right = temp;
			temp->left = current;
			temp->number = e;
			temp->up_left = current->up_left;
			temp->up_right = current->up_right;
			break;
		}
	}
	return temp;
}

n* n_next_level(n* current, int e)
{
	while (1)
	{
		if (e <= current->number) ///move to left son node
		{
			current = current->down_left;
			break;
		}
		else if (current->right != NULL)
		{
			current = current->right;
		}
		else                     ///move to right son node
		{
			current = current->down_right;
			break;
		}
	}
	return current;
}

void n_up_link_reset(n*current, n*left, n*right)
{
	while (current != NULL)
	{
		current->up_left = left;
		current->up_right = right;
		current = current->right;
	}
}


n*n_split(n**head, n*current)  //return pointer that point to upper node.
{
	while (current->left != NULL)
		current = current->left;  //move cursor to head node 
	current = current->right;     //send up second node 
	if (current->down_left != NULL)   //if down nodes exist. reset their up link      ///////
	{
		n_up_link_reset(current->down_left, current->left, NULL);
		n_up_link_reset(current->down_right, NULL, current->right);
	}
	current->down_left = current->left;
	current->down_right = current->right;  //fix current down link
	current->left = NULL;
	current->right = NULL;  //cut current side link
	current->down_left->right = NULL;
	current->down_right->left = NULL;  //cut 

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	if ((current->up_left == NULL) && (current->up_right == NULL))//head node
	{
		n_up_link_reset(current->down_left, NULL, current);
		n_up_link_reset(current->down_right, current, NULL);
		*head = current;
	}
	else if ((current->up_left != NULL) && (current->up_right == NULL))  //right side insert
	{
		n_up_link_reset(current->down_left, current->up_left, current);
		n_up_link_reset(current->down_right, current, NULL);//reset up link 
		current->up_left->right = current;
		current->left = current->up_left; //insert current to that level
		current->up_left = current->left->up_left;
		current->up_right = current->left->up_right;   //fix current's up link 
	}
	else if ((current->up_left == NULL) && (current->up_right != NULL))//left side insert, need to reset head or uplevel pointing
	{
		n_up_link_reset(current->down_left, NULL, current);
		n_up_link_reset(current->down_right, current, current->up_right);//reset up link 
		current->up_right->left = current;
		current->right = current->up_right; //insert current to that level
		current->up_left = current->right->up_left;
		current->up_right = current->right->up_right;   //fix current's up link           ////down link
		current->right->down_left = current->down_right;   //reset top level's down link


		if ((current->up_left == NULL) && (current->up_right == NULL))
			*head = current; //headnode link correction
		else
		{
			if (current->up_left != NULL)  //up level node link correction
				current->up_left->down_right = current;
			if (current->up_right != NULL)
				current->up_right->down_left = current;
		}
	}
	else//mid insert
	{
		n_up_link_reset(current->down_left, current->up_left, current);
		n_up_link_reset(current->down_right, current, current->up_right);  //reset up link 
		current->up_left->right = current;
		current->up_right->left = current;  
		current->left = current->up_left;
		current->right = current->up_right;  //insert current to that level
		current->right->down_left = current->down_right;  //reset top level's down link
		current->up_left = current->left->up_left;
		current->up_right = current->left->up_right;   //fix current's up link 
	}
	return current;
}


void n_insert(n**head_address, int e)
{
	n* temp = (n*)malloc(sizeof(n)); //memory allocation
	n* current = *head_address;
	int count;
	if (*head_address == NULL)       //empty
	{
		*head_address = temp;
		n_ini(temp);
		temp->number = e;
	}
	else
	{
		while (current->down_left != NULL)
			current = n_next_level(current, e);  //this will return next level head node (first node)
		current = n_level_insert(head_address, current, e);
		while (4 == n_count(current))
			current = n_split(head_address, current);
	}
}

//take care of buttom links
//one fusion //parent node two or more,   all siblin are one     //special fusion //not rotatable, siblin all one, parent node one
//one rotation //siblin at least one side is two or more, 
//normal delection,   no rotation no fusion, only delate leaf node, that left node maybe pointed by head 

n* get_tail(n* current)
{
	while (current->right != NULL)
		current = current->right;
	return current;
}

void one_rotation(n* current, int flag)//0 left   1 right       froms right, 
{
	n* parent=(flag) ? current->up_right : current->up_left;
	n* opponent = (flag) ? parent->down_right : get_tail(parent->down_left);
	if (flag)
	{
		if (current->down_left == NULL)
		{
			current->down_left = current->down_right;
			n_up_link_reset(current->down_left, NULL, current);
		}
		n_up_link_reset(opponent->down_left, current, NULL);
		current->down_right = opponent->down_left;
		n_up_link_reset(opponent->down_right, NULL, opponent->right);//linking work
		current->number = parent->number;
		parent->number = opponent->number;//move number
		parent->down_right = opponent->right;
		opponent->right->left = NULL;
		free(opponent);
	}
	else
	{
		if (current->down_right == NULL)
		{
			current->down_right = current->down_left;
			n_up_link_reset(current->down_right, current, NULL);
		}
		n_up_link_reset(opponent->down_right, NULL, current);
		current->down_left = opponent->down_right;
		n_up_link_reset(opponent->down_left, opponent->left, NULL);//linking work
		current->number = parent->number;
		parent->number = opponent->number;//move number
		opponent->left->right = NULL;
		free(opponent);
	}
}

n* one_fusion(n**head, n* current, int flag)//0 left   1 right       froms right, 
{
	n* parent = (flag) ? current->up_right : current->up_left;
	n* opponent = (flag) ? parent->down_right : parent->down_left;
	n* temp_head = (flag) ? current : opponent;////???

	int* recall_flag = 0;
	current->number = parent->number;
	if (flag)//from right
	{
		if (current->down_left == NULL)
		{
			current->down_left = current->down_right;
			n_up_link_reset(current->down_left, NULL, current);
		}
		n_up_link_reset(opponent->down_left, current, opponent);
		current->down_right = opponent->down_left;
		current->right = opponent;
		opponent->left = current;//link works
	}
	else//from left
	{
		if (current->down_right == NULL)
		{
			current->down_right = current->down_left;
			n_up_link_reset(current->down_right, current, NULL);
		}
		n_up_link_reset(opponent->down_right, opponent, current);
		current->down_left = opponent->down_right;
		current->left = opponent;
		opponent->right = current;//link works
	}
	
	if ((parent->left != NULL) && (parent->right != NULL))//////we have both side parent. then only from left fusion will hapen, 
	{
		parent->left->down_right = temp_head;
		parent->right->down_left = temp_head;
		n_up_link_reset(temp_head,parent->left, parent->right); //up down link set
		temp_head->up_left->right = temp_head->up_right;
		temp_head->up_right->left = temp_head->up_left; //re connect 
		free(parent);
	}
	else if ((parent->left == NULL) && (parent->right != NULL)) //change right side need to reset head or upper links
	{
		parent->right->down_left = temp_head;// head position
		n_up_link_reset(temp_head, NULL , parent->right);
		temp_head->up_right->left = NULL; //cut
		if ((parent->up_left == NULL) && (parent->up_right == NULL))
			(*head) = parent->right;
		else
		{
			if (parent->up_left != NULL)
				parent->up_left->down_right = parent->right;
			if (parent->up_right != NULL)
				parent->up_right->down_left = parent->right;
		}
		free(parent);
	}
	else if ((parent->left != NULL) && (parent->right == NULL)) //change leff side, parent at right tail, 
	{
		parent->left->down_right = temp_head; //1: from right,  n* temp_head = (flag) ? current : opponent;////??? 
		n_up_link_reset(temp_head, parent->left, NULL);
		temp_head->up_left->right = NULL;
		free(parent);
	}
	else//only one node up
	{
		parent->down_left = temp_head;  
		parent->down_right = NULL;
		n_up_link_reset(temp_head, NULL, parent);
		recall_flag = 1;
	}
	return (recall_flag) ? parent : NULL;
}

void normal_deletion(n** head, n* current) //leaf deletion, or node right under head
{
	if (1 == n_count(current)) //only one node case, near head
	{
		(*head) = current->down_left;
		n_up_link_reset(current->down_left,NULL,NULL);
		free(current);
	}
	else //only on leaf, also need to corrent their donw link
	{
		if (current->left == NULL)//left most
		{
			current->right->down_left = current->down_left; //link corrent
			if (current->down_left != NULL)
				n_up_link_reset(current->down_left, NULL, current->right);
			current->right->left = NULL;
			if ((current->up_left == NULL) && (current->up_right == NULL))
				(*head) = current->right;
			else
			{
				if (current->up_left != NULL)
					current->up_left->down_right = current->right;
				if (current->up_right != NULL)
					current->up_right->down_left = current->right;
			}
			free(current);
		}
		else if (current->right == NULL)//right most
		{
			current->left->down_right = current->down_left; //link corrent
			if (current->down_left != NULL)
				n_up_link_reset(current->down_left, current->left, NULL );
			current->left->right = NULL;
			free(current);
		}
		else//mid
		{
			current->left->down_right = current->down_left; //link corrent
			current->right->down_left = current->down_left; //link corrent
			if (current->down_left != NULL)
				n_up_link_reset(current->down_left, current->left, current->right);
			current->left->right = current->right;
			current->right->left = current->left;
			free(current);
		}
	}
}


n* n_search(n*current, int number)  //not exist return NULL
{
	while (1)
	{
		if (current == NULL)
			return NULL;
		if (number == current->number)
			return current;
		if (number < current->number) ///move to left son node
		{
			if (current->down_left == NULL)
				return NULL;
			current = current->down_left;
		}
		else if (current->right != NULL)
		{
			current = current->right;
		}
		else                     ///move to right son node
		{
			if (current->down_right == NULL)
				return NULL;
			current = current->down_right;
		}
	}
}

n* find_successor(n* current)
{
	current = current->down_left;
	current = get_tail(current);
	while (current->down_left != NULL)
	{
		current = current->down_right;
		current = get_tail(current);
	}
	return current;
}

int check_condition(n*current)
{
	if (2 <= n_count(current))
		return 0; //normal
	if ((current->up_left == NULL) && (current->up_right == NULL))
		return 0; //normal
	if ((current->up_left != NULL) && (current->up_right == NULL))
	{
		if (2 <= n_count(current->up_left->down_left))
			return 10; //rotation, from left to here
		else
			return 20; //fusion, from left side to here
	}
	else if ((current->up_left == NULL) && (current->up_right != NULL))
	{
		if (2 <= n_count(current->up_right->down_right))
			return 11; //rotation, from right to here
		else
			return 21; //fusion, from right side to here
	}
	else
	{
		if ((2 <= n_count(current->up_left->down_left)) && (2 <= n_count(current->up_right->down_right)))
			return 10;
		if ((2 <= n_count(current->up_left->down_left)) && (1 == n_count(current->up_right->down_right)))
			return 10;
		if ((1 == n_count(current->up_left->down_left)) && (2 <= n_count(current->up_right->down_right)))
			return 11;
		if ((1 == n_count(current->up_left->down_left)) && (1 == n_count(current->up_right->down_right)))
			return 20;
	}
}

n*n_leaf_delete(n** head, n* current)
{
	int flag = check_condition(current);
	//printf("%d",flag);
	n* result = NULL;
	switch (flag)
	{
	case 0: 
		normal_deletion(head, current);
		break;
	case 10: 
		one_rotation(current, 0);
		break;
	case 11:
		one_rotation(current, 1);
		break;
	case 20:
		result = one_fusion(head, current, 0);
		break;
	case 21:
		result = one_fusion(head, current, 1);
		break;
	default:
		break;
	}
	return result;
}


int n_delete(n** head, int number)
{
	n* position = n_search(*head, number);
	n* successor = NULL;
	n* check = NULL;
	if (position == NULL)
		return 1;  //1 means  not found.
	if (position->down_left == NULL)
	{
		check = position;
		do {
			check = n_leaf_delete(head, check);
		} while (check != NULL);
		return 0;
	}
	else
	{
		successor = find_successor(position);
		position->number = successor->number;
		check = successor;
		do {
			check = n_leaf_delete(head, check);
		} while (check != NULL);
		return 0;
	}
}



struct Q {
	struct Q* link;
	n* data;
};
typedef struct Q Q;

void head_print(Q**head, Q**down)
{
	n* current = (* head)->data;
	
	Q* tail = *down;
	Q* temp = *head;
	Q* temp2 = NULL;

	if (tail != NULL)
	{
		while (tail->link != NULL)
			tail = tail->link;
	}
	temp2 = (Q*)malloc(sizeof(Q));
	temp2->link = NULL;
	temp2->data = current->down_left;  //insert new Q to down 
	if ((*down) == NULL)
	{
		(*down) = temp2;
		tail = temp2;
	}
	else
	{
		tail->link = temp2;
		tail = tail->link;
	}
	printf("(");

	while (current != NULL)
	{
		if(current->right!=NULL)
			printf("%d ", current->number);
		else
			printf("%d) ", current->number);
		tail->link = (Q*)malloc(sizeof(Q));
		tail = tail->link;
		tail->data = current->down_right;
		tail->link = NULL;
		current = current->right;
	}

	*head = temp->link;
	free(temp);
}

void result_print(n*head)
{
	Q* up = (Q*)malloc(sizeof(Q));
	Q* down = NULL;
	Q* temp = NULL;
	up->data = head;
	up->link = NULL;
	if (head == NULL)
		return;
	do 
	{
		do 
		{
			head_print(&up, &down);
		} while (up != NULL);
		printf("\n");
		temp = up;
		up = down;
		down = temp;
	} while (up->data != NULL);
}


int main(void) {
	n* head = NULL;
	char command;
	int number;

	while (1) {
		printf("Enter command and number: ");
		scanf(" %c%d%c", &command, &number);  // Add a space before %c to consume newline
		//while (getchar() != '\n');  // Flush the input buffer

		if (command == 'i' ) {
			// Assuming n_insert is a function that inserts a node
			// Update this part with your actual implementation
			n_insert(&head, number);
			printf("Inserting %d\n", number);
			printf("\n");
		}
		else if (command == 'd') {
			// Assuming n_delete is a function that deletes a node
			// Update this part with your actual implementation
			n_delete(&head, number);
			printf("Deleting %d\n", number);
			printf("\n");
		}
		else
		{
			printf("Wrong instruction\n");
		}
		// Assuming result_print is a function that prints the result
		// Update this part with your actual implementation
		result_print(head);
		printf("\n");
		printf("Result printed\n");
	}

	return 0;
}














