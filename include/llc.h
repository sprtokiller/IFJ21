#pragma once
//dont use this!
//just for editors autocomplete
struct cluster_struct {
	char* data;
	struct cluster_struct* next;
};

//char cluster, an item of a linked list
typedef struct cluster_struct cluster_s;

//dont use this!
//just for editors autocomplete
struct ll_cluster_struct {
	size_t max_len;
	size_t total_len;
	size_t cluster_size;
	size_t active_cluster_len;
	cluster_s* start;
	cluster_s* active;
};

//linkedlist of char clusters
typedef struct ll_cluster_struct LLC_s;

/**
 * Initializes datastructure for dynamic allocation of text
 *
 * @param cluster_size The number of characters in a cluster
 * @return Pointer to initialized structure
 */
LLC_s* LLC_init(size_t cluster_size);

/**
 * Destroys datastructure for dynamic allocation of text
 *
 * @param llc Linked list of clusters
 */
void LLC_destroy(LLC_s* llc);

/**
 * Aappends new char to the list
 *
 * @param llc Linked list of clusters
 * @param c Character to be pushed
 * @return 0 on success, anything else on error.
 */
char LLC_push(LLC_s* llc, char c);

/**
 * Dumps the contents of llc into a single allocated string + '\0'
 *
 * @param llc Linked list of clusters
 */
char* LLC_dump(LLC_s* llc);
