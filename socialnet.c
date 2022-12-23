#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include<stdbool.h>
#define MAX 100

typedef struct node 
{
    int id; //ID of user
    int numfren; //number of friends of user
    char name[MAX]; //name of user
    int* friends; //friends of user as an array
    struct node* right;  //user to the right
    struct node* left; //user to the left
} node;

struct node* retUser(char str[MAX])
{
    char name[MAX];
    char ID[MAX];
    char strfriends[MAX];

    //copied ID
    char*token=strtok(str,",");
    strcpy(ID,token);

    //copied Name
    token=strtok(NULL,",");
    strcpy(name,token);

    //copied friends' ID
    token=strtok(NULL,",");
    strcpy(strfriends,token);
    //printf("%s\n",(strfriends));

    //creating user nodes
    int id=atoi(ID);
    struct node *user = malloc(sizeof(struct node));
    user->id=id;
    user->friends=(int*)malloc(MAX * sizeof(int));
    strcpy(user->name,name);


    //adding user's friend's IDs
    token = strtok(strfriends,"|");  
    int i=0;
    while( token != NULL ) 
    {

        int temp=atoi(token);
        user->friends[i]=temp;
        i++;
        token = strtok(NULL,"|");
    }
    user->numfren=i;
    if(i==0){user->friends[i]=-1;}
    return user;
}


//search for user with id=key
struct node* search(int key, struct node *users)
{
    if (users == NULL)
    {
        return NULL;
    }
    else 
    {
        while (users != NULL)
        {
            if (key < users -> id)
            {
                users = users -> left;
            }
            else if (key > users -> id)
            {
                users = users -> right;
            }
            else if(users -> id == key)
            {
                return users;
            }
        }
        return NULL;
    }
}

//see document for explanattion
struct node*refineUser(struct node*user, struct node *users)
{
    int id = user -> id;
    struct node* temp;
    int count = 0;
    int k = 0;
    bool isFriendPresent[MAX];
    int newFriends[MAX];
    struct node* foundid = user; 
    if (users == NULL)
    {
        user -> numfren = 0;
        user -> friends[0] = -1;
        return user;
    }
    
    while(foundid != NULL)
    {
        foundid = search(id++, users);
    }
    id--;
    user -> id = id;
    
    for (int i = 0; i < user -> numfren ; i++)
    {
        int friend = user -> friends[i];
        if (search (friend, users))
        {
            count++;
            isFriendPresent[i] = true;
            temp = search(friend, users);
            if (temp -> numfren == 0)
            {
                temp -> numfren++;
                temp -> friends[0] = id;
            }
            else
            {
                temp -> numfren++;
                temp -> friends[user -> numfren - 1] = id;
            }  
        }
        else 
        {
            isFriendPresent[i] = false;
        }
    }
    if (count == 0)
    {
        user -> numfren = 0;
        user -> friends[0] = -1;
    }
    else
    {
        for (int j = 0; j < user->numfren; j++)
        {
            if (isFriendPresent[j])
            {
                newFriends[k] = user -> friends[j];
                k++;
            }
        }
        for (int i = 0; i < k; i++)
        {
            user -> friends[i] = newFriends[i];
        }
        //user -> friends = newFriends;
        user -> numfren = k;
    }
    return user;
}

//insert user with id
struct node* insertUser(struct node*root,int id,struct node*user)
{
    user -> left = user -> right = NULL;
   if (root == NULL)
   {
        root = user;
        return root;
   }
   else 
   {
        struct node *prev, *cur;
        prev = NULL;
        cur = root;
        while (cur != NULL)
        {
            prev = cur;
            if (id < cur -> id)
            {
                cur = cur -> left;
            }
            else 
            {
                cur = cur -> right;
            }
        }
        if (id < prev -> id)
        {
            prev -> left = user;
        }
        else if (id > prev -> id)
        {
            prev -> right = user;
        }
        return root;
   }
}

//prints friends list
void friends(int id, struct node *users) 
{
   struct node *temp;
   temp = search(id, users);
   if (temp -> numfren == 0)
   {
        printf("-1\n");
   }
   else
    {
        for (int i = 0; i < temp -> numfren; i ++)
        {
            printf("%d \n", temp -> friends[i]);
        }
    }
}

//find child node with minimum value (inorder successor) - helper to delete node
struct node *minValueNode(struct node *node) 
{
    struct node* current;
    current = node;
    while (current && current->left != NULL)
        current = current->left;
  
    return current;
}

//deletes itself from its friend's nodes
struct node*deleteFriends(int key, struct node*users)
{
    struct node *temp, *friend;
    temp = search (key, users);
    if (temp != NULL)
    {
        int n = temp -> numfren;
        for (int i = 0; i < n; i ++)
        {
            friend = search(temp -> friends[i], users);
            if (friend != NULL)
            {
                if (friend -> numfren == 1)
                {
                    if (friend -> friends[0] == key)
                    {
                        friend -> numfren --;
                        friend -> friends[0] = -1;
                    }
                }
                else
                {
                    for (int j = 0; j < friend -> numfren; j++)
                    {
                        if (friend -> friends[j] == key)
                        {
                            for (int k = j; k < friend -> numfren; k++)
                            {
                                friend -> friends[k] = friend -> friends[k+1];
                            }
                        }
                    }
                    friend -> numfren--;
                } 
            }
        }
    }
    return NULL;
}

// Deleting a node
struct node *deleteNode(struct node *root, int key) 
{
    if (root == NULL)
        return root;
    if (key < root -> id)
        root -> left = deleteNode(root -> left, key);
    else if (key > root -> id)
        root -> right = deleteNode(root -> right, key);
    else 
        {
        if (root->left == NULL) 
        {
            struct node* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) 
        {
            struct node* temp = root->left;
            free(root);
            return temp;
        }
        struct node* temp = minValueNode(root->right);
        root -> id = temp -> id;
        strcpy(root -> name,temp -> name);
        for (int i = 0; i< temp -> numfren ; i++)
        {
            root -> friends[i] = temp -> friends[i];
        }
        root -> numfren = temp -> numfren;
        root->right = deleteNode(root->right, temp->id);
    }
    return root;
}

//Print USER's IDs in ascending order
void printInOrder(node* myusers) 
{
    if (myusers != NULL)
    {
        printInOrder(myusers -> left);
        printf("%d %s \n", myusers -> id, myusers -> name);
        printInOrder(myusers -> right);
    }
}


int main(int argc, char **argv)
{
    node *users=NULL;   
    while(1)
    {

        int opt, id;
        //fflush(stdin);
        scanf("%d",&opt);
        char str[MAX];
        switch (opt)
        {
            case 1:
      
                scanf("%s",str);
                struct node*tbins=retUser(str);
                tbins=refineUser(tbins, users);
                users=insertUser(users,tbins->id,tbins);
                break;

            case 2:
           
                scanf("%d", &id);
                deleteFriends(id, users);
                users=deleteNode(users, id);
                break;

            case 3:
        
                scanf("%d", &id);
                node* result=search(id,users);
                if(result==NULL) 
                    printf("USER NOT IN TREE.\n");
                else{
                    printf("%d\n",result->id);
                }
                break;

            case 4:
                scanf("%d", &id);
                friends(id, users);
                break;

            case 5:
                printInOrder(users);
                break;

            case 6:
                exit(0);
                break;

            default: 
                printf("Wrong input! \n");
                break;
        }

    }
    return 0;
}