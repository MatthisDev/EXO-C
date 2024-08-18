#include <stdlib.h>  
#include <stdio.h> // malloc - realloc 
#include <string.h> // strcmp

struct item_node {
    int id;
    const char* name;
    struct item_node* next;
};

void reverse_inventory(struct item_node** head);
void add_item(struct item_node** head, int id, const char* name);
void insert_item(struct item_node** head, int index, int id, const char* name);
void remove_item_by_index(struct item_node** head, int index);
void remove_item_by_name(struct item_node** head, const char* name);
void display_inventory(struct item_node* head);
void free_inventory(struct item_node* head);

int main() {
    struct item_node* inventory = NULL;

    add_item(&inventory, 1, "Master Sword");
    add_item(&inventory, 2, "Hylian Shield");
    add_item(&inventory, 3, "Boomerang");
	// 1 3
    insert_item(&inventory, 1, 4, "Hookshot"); // Insert "Hookshot" at index 1
    insert_item(&inventory, 3, 5, "Bombs"); // Insert "Bombs" at index 3

    display_inventory(inventory); // Expected output: ID: 1, Name: Master Sword -> ID: 4, Name: Hookshot -> ID: 2, Name: Hylian Shield -> ID: 5, Name: Bombs -> ID: 3, Name: Boomerang -> NULL

   	remove_item_by_index(&inventory, 2); // Remove the item at index 2 (Hylian Shield)
    remove_item_by_name(&inventory, "Bombs"); // Remove the first occurrence of "Bombs"

    display_inventory(inventory); // Expected output: ID: 1, Name: Master Sword -> ID: 4, Name: Hookshot -> ID: 3, Name: Boomerang -> NULL
	reverse_inventory(&inventory);

	display_inventory(inventory);

    free_inventory(inventory); // Clean up memory
    return 0;
}

void reverse_inventory(struct item_node** head){
	struct item_node* before_node = NULL;
	struct item_node* current_node = (*head);
	struct item_node* after_node = NULL;
	
	if(current_node != NULL)
		after_node = current_node->next;

	while(current_node != NULL){
		current_node->next = before_node;

		before_node = current_node;
		current_node = after_node;
		if(after_node != NULL)
			after_node = after_node->next;
	}
	
	(*head) = before_node;
}

void remove_item_by_name(struct item_node** head, const char* name){
	struct item_node* before_node = NULL;
	struct item_node* node = (*head);
	
	// we delete all occurrences  
	while(node != NULL){
		// when we find a node with the name
		if(strcmp(node->name, name) == 0){
			// we link the node before with the node after
			if(before_node != NULL)
				before_node->next = node->next;
			// AND we delete it
			free(node);
		}
		
		before_node = node;
		node = node->next;
	}
}

void remove_item_by_index(struct item_node** head, int index){
	struct item_node* before_node = NULL;
	struct item_node* node = (*head);
	// we find the node to delete while saving the node before it	
	for(int i = 0; i < index; ++i){
		before_node = node;
		node = node->next;
	}

	if(before_node != NULL)
		// now the node before is link to the node after
		before_node->next = node->next;
	
	// Dont forget to free mem
	free(node);
}

void insert_item(struct item_node** head, int index, int id, const char* name){
	// we create the node
	struct item_node* node = malloc(sizeof(struct item_node));
	node->name = name;
	node->id = id;
	node->next = NULL;

	// we put the node at the right place in the list	
	struct item_node* before_node = NULL;
	struct item_node* current_node = (*head);
	
	for(int i = 0; i < index; i++){
		before_node = current_node;
		current_node = current_node->next;	
	}
	
	if(before_node == NULL){
		*head = node;	
	}else
		before_node->next = node;
	
	// the node at the current I index become the node a I+1 index	
	node->next = current_node;
	
}

void add_item(struct item_node** head, int id, const char* name){
	struct item_node* before_node = NULL;
	struct item_node* current_node = (*head);
	// We create the node to be added at the end off the list
	struct item_node* new_node = malloc(sizeof(struct item_node));
	new_node->id = id;
	new_node->name = name;
	new_node->next = NULL;

	while(current_node != NULL){
		before_node = current_node;
		current_node = current_node->next;
	}
	
	if(before_node != NULL)
		before_node->next = new_node;
	// Handle extra case (when the inventory is empty)
	else
		(*head) = new_node;
}

void display_inventory(struct item_node* head){
	while(head != NULL){
		printf("%s [%d] \n", head->name, head->id);
		if(head->next != NULL)
			printf("|\nV\n");

		head = head->next;
	}
	printf("\n");
}


void free_inventory(struct item_node* head){
	if(head != NULL){
		// recursivement on parcout toute la chaine pour desallouer l'element au bout de maniere 
		free_inventory(head->next);
		free(head);
	}
}
