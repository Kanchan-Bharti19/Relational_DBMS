#include "BPtree.h"

class Btreenode
{

  private:
    bool leaf;
    std::vector < int >pointers;
    std::vector < int >keys;
    int next_node;

  public:
      Btreenode();
      Btreenode(bool makeleaf){
        leaf = makeleaf;
        next_node = -1;
    }

    bool isleaf(){
        return leaf;
    }

    void set_leaf(bool val){
        leaf = val;
    }

    int num_keys(){
        return keys.size();
    }

    int num_pointers(){
        return pointers.size();
    }

    int get_key(int i){
        return keys[i - 1];
    }

    int get_pointer(int i){
        return pointers[i - 1];
    }

    void set_next(int x){
        next_node = x;
    }

    int get_next(){
        return next_node;
    }

    void push_key(int val){
        keys.push_back(val);
    }

    void push_pointer(int val){
        pointers.push_back(val);
    }

    void clear_data(){
        keys.clear();
        pointers.clear();
    }

    int* get_keys_add(){
        return &keys[0];
    }

    int* get_point_add(){
        return &pointers[0];
    }

    ~Btreenode(){
        clear_data();
    }

    /*Function that performs binary search and returns the
       correct child. For Internal Nodes */
    /*
    std::lower_bound()
      Returns an iterator pointing to the first element in
       the range [first,last) which does not compare less than val.
       If all the element in the range compare less than val, the
       function returns last.
    */
    int get_next_key(int search_key){
        return (std::lower_bound(keys.begin(), keys.end(),
                                 search_key) - keys.begin());
    }

    /*Function that performs binary search and returns
       boolean result */
    int search_key(int search_key){
        return std::binary_search(keys.begin(), keys.end(), search_key);
    }

    //Function to check if node is full
    bool full(){
        if (leaf){
            if (pointers.size() < BPTREE_MAX_KEYS_PER_NODE - 1)
                return false;
            else
                return true;
        }
        if (pointers.size() < BPTREE_MAX_KEYS_PER_NODE)
            return false;
        else
            return true;
    }

    /*Function that inserts a new record in a node which
       is not yet full */
    void insert_key(int key, int point){
        //get the position in vector keys to insert
		//new key so that keys[] remains sorted;
        int pos = get_next_key(key);
        keys.insert(keys.begin() + pos, key);
        //insert position of key inside pointers;
        if (leaf)
            pointers.insert(pointers.begin() + pos, point);
        else
            pointers.insert(pointers.begin() + pos + 1, point);
    }

    /*Function that copies first 'n' records from a given
       node to the current node */
    void copy_first(Btreenode & node, int n){
        keys.clear();
        pointers.clear();
        for (int i = 1; i < n; i++){
            keys.push_back(node.get_key(i));
            pointers.push_back(node.get_pointer(i));
        }
        pointers.push_back(node.get_pointer(n));
        if (leaf){
            keys.push_back(node.get_key(n));
        }
    }

    /*Function that copies records from a given node to
       current node. It ignores first 'n' records */
    void copy_last(Btreenode & node, int n){
        keys.clear();
        pointers.clear();
        int lim = node.num_pointers();
        for (int i = n + 1; i <= lim; i++){
            pointers.push_back(node.get_pointer(i));
        }
        lim = node.num_keys();
        for (int i = n + 1; i <= lim; i++){
            keys.push_back(node.get_key(i));
        }
    }

    //Overloading write operator to write to a file
    /*
		write to file in format
		(is_leaf_or_not,keys_size,for(0 to key_size)<keys_Values>,pointer_size,for(0to pointers_size)<pointer_values>, next_node);

		according to sample program
		tree0.dat will store data in following format
		(1 8 1 3 5 6 7 8 9 12 8 2 4 1 7 3 0 6 5 -1);
		*/
    friend std::ofstream & operator<<(std::ofstream & os, const Btreenode & en){
        // is leaf or not;
        os << en.leaf << " ";
        // keys size
        os << (int) en.keys.size() << " ";
        for (unsigned int i = 0; i < en.keys.size(); i++){
            os << en.keys[i] << " ";
        }
        os << (int) en.pointers.size() << " ";
        // pointers are the indices of the stored elements in vector a;
        for (unsigned int i = 0; i < en.pointers.size(); i++){
            os << en.pointers[i] << " ";
        }
        os << en.next_node;
        return os;
    }

    //Overloading read operator to read from a fil
    friend std::ifstream & operator>>(std::ifstream & is, Btreenode & en){
        // read all the data stored in file <descrbed in << operator overloading>
        int ts;
        is >> en.leaf;
        is >> ts;
        en.keys.resize(ts);
        for (int i = 0; i < ts; i++){
            is >> en.keys[i];
        }
        is >> ts;
        en.pointers.resize(ts);
        for (int i = 0; i < ts; i++){
            is >> en.pointers[i];
        }
        is >> en.next_node;
        return is;
    }

    //Overloading Assignment Operator
    Btreenode & operator=(const Btreenode & n){
        if (this != &n){
            leaf = n.leaf;
            keys.assign(n.keys.begin(), n.keys.end());
            pointers.assign(n.pointers.begin(), n.pointers.end());
        }
        return *this;
    }
};

    /* Function that writes a node 'n' to a given
       filenum(like pointer) */
void BPtree :: write_node(int filenum, Btreenode n){
    char *str;
    str = (char *) malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
    //if filenum=0; open tree0.dat;
    sprintf(str, "table/%s/tree/tree%d.dat", tablename,filenum);
    std::ofstream out_file(str, std::ofstream::binary |  std::ofstream::out | std::ofstream::trunc);
    free(str);
    //write data to out_file root = n()
    out_file << n;
    out_file.close();
}


    /* Function that updates Meta-Data of a table after
       inserting a record */
void BPtree :: update_meta_data(){
    char *str;
    str = (char *) malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
    sprintf(str, "table/%s/tree/meta_tree.dat", tablename);
    std::ofstream out_file(str, std::ofstream::out | std::ofstream::trunc
                                        | std::ofstream::binary );
    out_file << files_till_now << " " << root_num;
    out_file.close();
    free(str);
}


    /* Constructor function that initializes tree with existing
       meta-data or creates new tree and writes meta-data */
BPtree :: BPtree(char table_name[]){
    strcpy(tablename,table_name);
    char *str;
    str = (char *) malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
    sprintf(str, "mkdir -p table/%s/tree", tablename);
    // cout<<"str"<<str<<endl;
    system(str);
    sprintf(str, "table/%s/tree/meta_tree.dat", tablename);

    std::ifstream in_file(str, std::ifstream::in | std::ifstream::binary);
    if (!in_file){
            /* If no meta data for table's BTree is found i.e.,
               It is new table. Then Create new meta data */
        std::ofstream out_file(str, std::ofstream::binary |
                                   std::ofstream::out | std::ofstream::trunc);
        if (!out_file){
            printf("Critical Error : Unable to Write on Disk !!");
            printf("\nAborting ... ... \n");
            abort();
        }
        files_till_now = root_num = 0;
        out_file.write((char *) (&files_till_now), sizeof(files_till_now));
        out_file.write((char *) (&root_num), sizeof(root_num));
        out_file.close();

        //initialize with root node = leaf node;
        Btreenode root(true);
        //set next_node=-1; as it is root;
        root.set_next(-1);
        //write node data to outfile;
        write_node(0, root);
    }//if file already created read the previously stored data;
    else{
        /* Read old Meta Data */
        in_file >> files_till_now >> root_num;
        in_file.close();
    }
    free(str);
}

/* Function that reads a node 'n' from a given
      filenum(like pointer) */
void BPtree :: read_node(int filenum, Btreenode & n){
    char *str;
    str = (char *) malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
    sprintf(str, "table/%s/tree/tree%d.dat", tablename, filenum);
    std::ifstream in_file(str, std::ifstream::in | std::ifstream::binary);
    free(str);
    in_file >> n;
    in_file.close();
}


/* Function that traverses the BPtree and returns the leaf node
    where 'primary_key' record should exist, if it exist at all */
Btreenode BPtree::search_leaf(int primary_key){
    Btreenode n(true);
    int q, curr_node = root_num;
    read_node(curr_node, n);

    //Traversing the Tree from root till leaf
    while (!n.isleaf()){
        q = n.num_pointers();
        if (primary_key <= n.get_key(1)){
            //set curr_node =pointers[0];
            curr_node = n.get_pointer(1);
        }
        else if (primary_key > n.get_key(q - 1)){
            //set curr_node = pointers[q-1];
            curr_node = n.get_pointer(q);
        }else{
            //find the correct position of key to be stored;
            curr_node = n.get_pointer(n.get_next_key(primary_key) + 1);
        }
        read_node(curr_node, n);
    }
    return n;
}

/* A function that returns the record number of a tuple
    with indexed column = 'primary_key' */
int BPtree::get_record(int primary_key){
	clock_t start=clock();
    Btreenode n = search_leaf(primary_key);
    int pos = n.get_next_key(primary_key) + 1;
    clock_t stop=clock();
    double elapsed=(double)(stop-start)*1000.0/CLOCKS_PER_SEC;
    printf("\nTime elapsed for search is %f ms\n",elapsed);
    if (pos <= n.num_keys() && n.get_key(pos) == primary_key){
        return n.get_pointer(pos);
    }else{
        return BPTREE_SEARCH_NOT_FOUND;
    }
}

/* A function the inserts a (key, record_num) pair in the
    B+ Tree */
// key is first coloumn of database either can be int or varchar;
int BPtree::insert_record(int primary_key, int record_num){
	 //printf("pri %d\n record_num %d",primary_key,record_num);
   // Btreenode n= leaf=true, next_node =-1;
    Btreenode n(true);
    int q, j, prop_n, prop_k, prop_new, curr_node = root_num;
    bool finish = false;
    std::stack < std::pair<int, Btreenode> > S;
    // read all the data of node stored in file tree%d.data (%d==curr_node=root_num=file_no);
	  // now n contains all the previously stored data;
    read_node(curr_node, n);

    // Traverse the tree till we get the leaf node;
    while (!n.isleaf()){
        S.push(make_pair(curr_node,n));      // Storing address in case of split
        // num_pointers==function that returns size of pointers vector from the block file;
        q = n.num_pointers();
        if (primary_key <= n.get_key(1)){
            curr_node = n.get_pointer(1);
        }else if (primary_key > n.get_key(q - 1)){
            curr_node = n.get_pointer(q);
        }else{
            curr_node = n.get_pointer(n.get_next_key(primary_key) + 1);
        }
        // get all the data of node n from file tree%d.dat(%d==curr_node);
        read_node(curr_node, n);
    }

    // Here n is Leaf Node
    // if key already exists then return ERROR
    if (n.search_key(primary_key)) {
        return BPTREE_INSERT_ERROR_EXIST;
    }

    /*
      if n is not full, insert key and pointer to node
      write back node to file, update meta_data and return
    */
    if (!n.full()){
        n.insert_key(primary_key, record_num);
        write_node(curr_node, n);
        update_meta_data();
        return BPTREE_INSERT_SUCCESS;
    }

    //if node n is full, then split;
    Btreenode temp(true), new_node(true);

    temp = n;
    temp.insert_key(primary_key, record_num);
    j = ceil((BPTREE_MAX_KEYS_PER_NODE + 1.0) / 2.0);
    // if max_key_per_node = 4, j = 3
    // copy the first j values of temp to node n
    // and remaining to new_node
    n.copy_first(temp, j);
    //now one file is increased to store the new node;
    files_till_now++;
    // next pointer of new_node will be next pointer of n
    // and next pointer of n will be newly created node new_node
    new_node.set_next(n.get_next());
    n.set_next(files_till_now);
    new_node.copy_last(temp, j);

    /*
      prop_k is key to be inserted into root
      and prop_new and prop_n are pointers to
      be attached to this root value
    */
    prop_k = temp.get_key(j); // key to be moved to new root
    prop_new = files_till_now;
    prop_n = curr_node;

    // write back the two new nodes created
    write_node(files_till_now, new_node);
    write_node(curr_node, n);
    temp.clear_data();
    new_node.clear_data();

    /* Keep repeating until we reach root
       or find an empty internal node */
    while (!finish){
        if (S.size() == 0){
            /*Last element splitted was root
               so create new root and assign meta_data */
            Btreenode nn(false);
            // insert key to new root
            nn.push_key(prop_k);
            // insert two pointer associated to this key
            // for left and right
            nn.push_pointer(prop_n);
            nn.push_pointer(prop_new);
            files_till_now++;
            write_node(files_till_now, nn);
            root_num = files_till_now;
            finish = true;
        }else{
            std::pair<int, Btreenode> p = S.top();
            curr_node = p.first;
            n = p.second;
            S.pop();
            // read_node(curr_node, n);
            if (!n.full()){
                n.insert_key(prop_k, prop_new);
                write_node(curr_node, n);
                finish = true;
            }else{
                /* Split is propogating towards top */
                temp = n;
                temp.insert_key(prop_k, prop_new);
                j = floor((BPTREE_MAX_KEYS_PER_NODE + 1.0) / 2.0);
                n.copy_first(temp, j);
                files_till_now++;
                new_node.set_leaf(false);
                new_node.copy_last(temp, j);
                write_node(files_till_now, new_node);
                write_node(curr_node, n);
                prop_k = temp.get_key(j);
                prop_new = files_till_now;
                prop_n = curr_node;
            }
        }
    }

    update_meta_data();
    return BPTREE_INSERT_SUCCESS;
}
