#include "search_tree.h"

struct tree_node {
    search_tree_type_t data;

    tree_node_t *smaller;
    tree_node_t *bigger;
};

tree_node_t *tree_node_new();
void tree_node_deep_delete(tree_node_t *node);

size_t tree_node_first_letter_count(tree_node_t *node, char first_letter);

search_tree_t *search_tree_new() {
    search_tree_t *tree = NULL;
    if (my_calloc((void *)&(tree), 1, sizeof(search_tree_t *)) != 0)
        return NULL;
    return tree;
}

void search_tree_delete(search_tree_t *tree) {
    search_tree_clear(tree);

    my_free(tree);    
}

void search_tree_clear(search_tree_t *tree) {
    if (tree == NULL || *tree == NULL)
        return;

    *tree = NULL;

    tree_node_deep_delete(*tree);
}

void search_tree_pre_order(search_tree_t *tree, void (*function)(void *data)) {
    if (tree == NULL || *tree == NULL)
        return;
    
    function((*tree)->data);
    search_tree_pre_order(&((*tree)->smaller), function);
    search_tree_pre_order(&((*tree)->bigger), function);
}

void search_tree_in_order(search_tree_t *tree, void (*function)(void *data)) {
    if (tree == NULL || *tree == NULL)
        return;
    
    search_tree_in_order(&((*tree)->smaller), function);
    function((*tree)->data);
    search_tree_in_order(&((*tree)->bigger), function);
}

void search_tree_post_order(search_tree_t *tree, void (*function)(void *data)) {
    if (tree == NULL || *tree == NULL)
        return;
    
    search_tree_post_order(&((*tree)->smaller), function);
    search_tree_post_order(&((*tree)->bigger), function);
    function((*tree)->data);
}

int search_tree_is_empty(search_tree_t *tree) {
    return *tree == NULL;
}

int search_tree_insert(search_tree_t *tree, search_tree_type_t value, comparator_f comparator) {
    if (tree == NULL)
        return 1;

    tree_node_t **current = tree;
    while (*current != NULL) {
        int rc = comparator(value, (*current)->data);
        if (rc < 0)
            current = &(*current)->smaller;
        else if (rc > 0)
            current = &(*current)->bigger;
        else
            return 0 ;
    }

    *current = tree_node_new();
    (*current)->data = value;
    if (current == NULL)
        return 2;

    return 0;
}

tree_node_t *tree_node_remove(tree_node_t *root, search_tree_type_t key, comparator_f comparator) {
    if (root == NULL)
        return root;

    int cmp = comparator(key, root->data);

    if (cmp < 0)
        root->smaller = tree_node_remove(root->smaller, key, comparator);
    else if (cmp > 0)
        root->bigger = tree_node_remove(root->bigger, key, comparator);
    else {
        if (root->smaller == NULL) {
            tree_node_t* tmp = root->bigger;
            free(root);
            return tmp;
        }
        else if (root->bigger == NULL) {
            tree_node_t* tmp = root->smaller;
            free(root);
            return tmp;
        }

        tree_node_t *tmp = root->bigger;

        while (tmp->smaller != NULL)
            tmp = tmp->smaller;

        root->data = tmp->data;
        root->bigger = tree_node_remove(root->bigger, tmp->data, comparator);
    }
    return root;
}


int search_tree_remove(search_tree_t *tree, search_tree_type_t value, comparator_f comparator) {
    if (tree == NULL)
        return 1;
    
    *tree = tree_node_remove(*tree, value, comparator);
    return 0;
}

int tree_node_search(tree_node_t *node, search_tree_type_t value, comparator_f comparator) {
    if (node == NULL)
        return 0;
    
    int rc = comparator(value, node->data);
    if (rc == 0)
        return 1;
    
    if (rc > 0)
        return tree_node_search(node->bigger, value, comparator);
    
    return tree_node_search(node->smaller, value, comparator);
}

int search_tree_search(search_tree_t *tree, search_tree_type_t value, comparator_f comparator) {
    return tree_node_search(*tree, value, comparator);
}

size_t search_tree_first_letter_count(search_tree_t *tree, char first_letter) {
    return tree_node_first_letter_count(*tree, first_letter);
}

size_t tree_node_first_letter_count(tree_node_t *node, char first_letter) {
    if (node == NULL)
        return 0;

    size_t count = 0;
    
    count += tree_node_first_letter_count(node->smaller, first_letter);
    count += (node->data[0] == first_letter);
    count += tree_node_first_letter_count(node->bigger, first_letter);

    return count;    
}

void tree_node_to_dot(FILE *stream, tree_node_t *node, char first_letter) {
    if (node == NULL)
        return;
    if (node->data[0] == first_letter)
        fprintf(stream, "\t\"%s\" [color=\"lightgreen\",style=\"filled\"]\n", node->data);
    if (node->smaller != NULL) {
        fprintf(stream, "\t\"%s\" -> \"%s\" [color=\"blue\"]\n", node->data, node->smaller->data);
        tree_node_to_dot(stream, node->smaller, first_letter);
    }
    if (node->bigger != NULL) {
        fprintf(stream, "\t\"%s\" -> \"%s\" [color=\"red\"]\n", node->data, node->bigger->data);
        tree_node_to_dot(stream, node->bigger, first_letter);
    }
}

void search_tree_to_dot(FILE *stream, search_tree_t *tree) {
    fprintf(stream, "digraph tree {\n");

    tree_node_to_dot(stream, *tree, '\0');

    fputs("}\n", stream);
}

void search_tree_to_dot_first_letter(FILE *stream, search_tree_t *tree, char firlst_letter) {
    fprintf(stream, "digraph tree {\n");

    tree_node_to_dot(stream, *tree, firlst_letter);

    fputs("}\n", stream);
}

tree_node_t *tree_node_new() {
    tree_node_t *node = NULL;
    if (my_calloc((void *)&node, 1, sizeof(sizeof(tree_node_t))) != 0)
        return NULL;
    
    return node;
}

void tree_node_deep_delete(tree_node_t *node) {
    if (node == NULL)
        return;
    
    tree_node_deep_delete(node->smaller);
    tree_node_deep_delete(node->bigger);

    my_free(node);
}
