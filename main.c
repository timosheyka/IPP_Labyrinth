#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include <ctype.h>

#define TestBit(A,k)    ( A[(k/32)] & (1 << (k%32)) )
#define SetBit(A,k)     ( A[(k/32)] |= (1 << (k%32)) )

// Node structure
typedef struct Queue{
    long long idx;
    long long dist;
    struct Queue * next;
}Queue;

// Lim structure , defines the bounds of queue
typedef struct Lim {
    struct Queue * first;
    struct Queue * last;
}Lim;

/* getLine() using getchar() converts input to array of nums
 * param[in] - array to be filled
 * return - the amount of elements in array
 */
int getLine(long long * nums) {
    int n = 0;
    int x;
    long long cur = 0;
    long long ind = 1;
    int k = 0;
    bool first = true;
    while (true)
    {
        x = getchar();
        if (!isdigit(x) && !isspace(x) && x != '\t') return -1;
        while(first && (x == ' ' || x == '\t'))
            if (!isdigit(x) && !isspace(x) && x != '\t')
                x = getchar();
            else
                return -1;
        if(first && x == '\n') return -1;
        first = false;
        if (x == ' ' || x == '\n' || x == '\t')
        {
            if (cur != 0)
            {
                nums[n++] = cur;
                int pt = 0;
                while (cur) { cur /= 10; pt++; }
                ind = 1;
                long long sk = 1;
                while (sk <= nums[n - 1])
                    sk *= 10;
                sk /= 10;
                long long temp = 0;
                while (nums[n - 1])
                {
                    temp = temp + (nums[n - 1] % 10) * sk;
                    sk /= 10;
                    nums[n - 1] /= 10;
                }
                nums[n - 1] = temp;
                while (pt < k) { nums[n - 1] *= 10; pt++; }
                k = 0;
            }
            if (x == '\n') break;
            continue;
        }
        long long * a;
        long long tmp = cur;
        a = &tmp;
        if(__builtin_saddll_overflow(cur, (x - '0') * ind, a)) { return -1; }
        cur += (x - '0') * ind;
        ind *= 10;
        k++;
    }
    if (n == 0) return 1;
    return n;
}

/* Converts int num to binary char *
 *
 * param[in] - num from [0..15]
 * return num representation with 4 bit char
 */
char * DecimalToBinary(int num) {
    char *binArr = malloc(sizeof(char) * 4);
    binArr[0]='0';
    binArr[1]='0';
    binArr[2]='0';
    binArr[3]='0';
    for (int i = 0; num > 0;) {
        binArr[i++] = (char) ('0' + (num % 2));
        num /= 2;
    }
    char tmp = binArr[0];
    binArr[0] = binArr[3];
    binArr[3] = tmp;
    tmp = binArr[1];
    binArr[1] = binArr[2];
    binArr[2] = tmp;

    return binArr;
}

/* Convert char from 'A' to 'F' to num
 * param[in] - char s
 * return int representation of certain char
 */
int CharToNum(char s) {
    switch (s) {
        case 'A': return 10;
        case 'B': return 11;
        case 'C': return 12;
        case 'D': return 13;
        case 'E': return 14;
        case 'F': return 15;
        default: return -1;
    }
}

/* Converts 4th line 16bit number to bitArr elements
 * param[in] - bit array
 * para[in] - product of defined dimensions values
 * return true , when conversion is possible , otherwise stops the program
 */
bool SxtnTo2bit(int ** bitArr, long long product) {
    int x;
    int count = 0;
    while ((x = getchar()) != '\n') {
        if (!isalnum(x) && !isspace(x) && x != '\t') { printf("ERROR 4\n"); return false; }
        if (isdigit(x)) {
            char * bit = DecimalToBinary(x - '0');
            for (int i = 0; i < 4; i++)
                if ((bit[i] - '0') == 1)
                    (*bitArr[((product - 1 - (count * 4 + i))/32)] =
                              *bitArr[((product - 1 - (count * 4 + i))/32)] |
                              (1 << ((product - 1 - (count * 4 + i))%32)));
            count++;
            free(bit);
        }
        else {
            if (!isspace(x) && x != '\t') {
                int num = CharToNum(x);
                if (num < 0) { printf("ERROR 4\n"); return false; }
                char * bit = DecimalToBinary(num);
                for (int i = 0; i < 4; i++)
                    if ((bit[i] - '0') == 1)
                        SetBit(*bitArr, (product - 1 - (count * 4 + i)));
                count++;
                free(bit);
            }
        }
    }
    if(x == '\n' || x == '\t') return false;
    return true;
}
/* Converts five numbers given in the 4th line to bit array elements
 * param[in] - bit array
 * para[in] - product of defined dimensions values
 */
void RTo2bit(int ** bitArr, long long product) {

    long long * nums = (long long *) malloc(sizeof(long long) * 5);
    int count = getLine(nums);
    if (count < 5) { printf("ERROR 4\n"); return; }
    long long twto32 = 2 * ((long long) INT_MAX + 1);

    long long * s = (long long *) malloc(sizeof(long long) * nums[3]);
    if (nums[2] == 0) s[0] = 0;
    else s[0] = (nums[0] * nums[4] + nums[1]) % nums[2];
    for (int i = 1; i < nums[3]; i++)
        s[i] = (nums[0] * s[i - 1] + nums[1]) % nums[2];

    long long * w = (long long *) malloc(sizeof(long long) * nums[3]);
    w[0] = s[0] % product;
    for (int i = 1; i < nums[3]; i++)
        w[i] = s[i] % product;

    size_t x;
    for(int i = 0; i < nums[3]; i++) {
        int k = 0;
        do {
            x = (w[i] + twto32 * k++);
            (*bitArr)[(x/32)] |= (1 << (x/32));
        }while(x + twto32 < product);
    }
    free(nums);
    free(s);
    free(w);
}

int address() {
    int way = 0;
    int x;
    while ((x = getchar()) == ' ');
    if (x == '0') {
        while ((x = getchar()) == ' ');
        if (x == 'x') way = 1;
        else return 0;
    }
    else
        if (x == 'R') way = 2;
        else return 0;
    return way;
}

/* Counting expression given in the task
 * param[in] - array of dimensions values
 * param[in] - array of certain coordinates
 * return - the index in bit array
 */
long long expression(const long long * nums, int n, const long long * coordinates) {
    long long ans = 0;
    for (int i = 0; i < n; i++) {
        long long *res;
        long long start = (coordinates[i] - 1);
        res = &start;
        for (int j = 0; j <= i - 1; j++)
            if (i > 0)
                if(__builtin_smulll_overflow(*res, nums[i], res))
                *res *= nums[j];
        ans += *res;
    }
    return ans;
}
/* Setting the product of whole dimensions values
 * param[in] - array of dimensions values
 * param[in] - the count of dimensions
 */
long long setProduct(const long long * nums, int n) {
    long long * res;
    long long start = 1;
    res = &start;
    for (int i = 0; i < n; i++)
       if(__builtin_smulll_overflow(*res, nums[i], res))  *res *= nums[i];
    return (*res);
}

/*
 * Corresponded to the task queue implementation
 */
bool isEmpty(Queue * queue) { return (queue->next == NULL); }

bool Initialise(Queue ** queue, Lim ** lim) {
    *queue = (Queue *) malloc(sizeof(Queue));
    if (!(*queue)) return false;
    (*queue)->next = NULL;
    (*queue)->dist = 0;

    *lim = (Lim *) malloc(sizeof(Lim));
    if (!(*lim)) return false;
    (*lim)->first = * queue;
    (*lim)->last = * queue;

    return true;
}

Queue * newNode(long long idx, long long dist)
{
    Queue * temp = (Queue *) malloc(sizeof(Queue));
    if (!temp) return NULL;
    temp->idx = idx;
    temp->dist = dist;
    temp->next = NULL;
    return temp;
}

void Push(Queue ** queue , Lim ** lim, long long idx, long long dist) {
    Queue * queue1 = newNode(idx, dist);
    (*lim)->last->next = queue1;
    (*lim)->last = queue1;
}

void Pop(Queue ** queue, Lim ** lim) {
    if(!isEmpty(*queue)) {
        if((*queue)->next == (*lim)->last) { (*lim)->last = *queue; }
        Queue * queueNext = (*queue)->next->next;
        free((*queue)->next);
        (*queue)->next = queueNext;
    }
}

Queue * Top(Queue * queue) {
    return queue->next;
}

void Clear(Queue ** queue, Lim ** lim) {
    free(*lim);
    Queue * queue1 = (*queue);
    Queue * queue2;
    while(queue1 != NULL) {
        queue2 = queue1->next;
        free(queue1);
        queue1 = queue2;
    }
}

/*
 * SetNeighbour() finds and sets the node neighbours
 */
 void SetNeighbour(Queue ** queue, Lim ** lim, int * bitArr, int k, long long dist, bool * found, const long long * nums, long long product, long long idx_out) {
    long long move = 1;
    long long check = nums[0];
    long long idx_in = (*lim)->first->next->idx;
    int i = 1;

    while (i <= k && !(*found)) {
        long long tmp = idx_in - move;
        if(tmp >= 0 && !TestBit(bitArr, tmp) && (idx_in/check == tmp/check)) {
            Push(queue, lim, tmp, dist);
            SetBit(bitArr, tmp);
            if (tmp == idx_out) *found = true;
        }
        tmp = idx_in + move;
        if(tmp <= product && !TestBit(bitArr, tmp) && (idx_in/check == tmp/check)) {
            Push(queue, lim, tmp, dist);
            SetBit(bitArr, tmp);
            if (tmp == idx_out) *found = true;
        }
        move = check;
        check *= nums[i++];
    }
}

/*
 * bfs implementation finding the shortest way between two points in bit array , if it exists
 */
long long bfs(int * bitArr, long long * nums, int n, long long idx_in, long long idx_out, long long product) {
    if(idx_in == idx_out && !TestBit(bitArr, idx_in)) return 0;
    long long ans = 0;
    bool found = false;

    Queue * queue;
    Lim * lim;
    if (!Initialise(&queue, &lim)) return -2;
    Push(&queue, &lim, idx_in, 0);

    while(!isEmpty(queue) && !found) {
        SetNeighbour(&queue, &lim, bitArr, n, Top(queue)->dist + 1, &found, nums, product, idx_out);
        if(Top(queue) != NULL) ans = Top(queue)->dist + 1;
        Pop(&queue, &lim);
    }

    Clear(&queue, &lim);
    if (found == false) return -1;
    return ans;
}

int main() {
    // 1st line input
    long long nums[100000];
    int n = getLine(nums);
    for(int i = 0; i < n; i++)
        if(nums[i] <= 0) { printf("ERROR 1\n"); return 1; }
    if (n < 0) { printf("ERROR 1\n"); return 1; }

    // setting dimensions product and allocate bit array
    long long product = setProduct(nums, n);
    long long size = product/32 + 1;
    int *bitArr = calloc(size, sizeof(int));

    // 2nd line input
    long long coordinates_in[n];
    int k1 = getLine(coordinates_in);
    if (k1 != n) { printf("ERROR 2\n"); return 1; }
    // counting entrance point
    long long idx_in = expression(nums, n, coordinates_in);

    // 3rd line input
    long long coordinates_out[n];
    int k2 = getLine(coordinates_out);
    if (k2 != n) { printf("ERROR 3\n"); return 1; }
    // counting exiting point
    long long idx_out = expression(nums, n, coordinates_out);

    // 4th line input
    // deciding which way is 4th is given
    int way = address();
    bool way1 = true;
    if (way > 0) {
        if (way == 1) { way1 = SxtnTo2bit(&bitArr, product); }
        if (way == 2) { RTo2bit(&bitArr, product); }
    } else { printf("ERROR 4\n"); return 1; }
    if (!way1) return 1;

    // checking possible 5th line
    if (getchar() == EOF) { printf("ERROR 5\n"); return 1; }

    // using normal bfs to count the shortest pass between points if it exists
    long long ans = bfs(bitArr, nums, n, idx_in, idx_out, product - 1);
    if (ans >= 0) { printf("%lld\n", ans); }
    else { if (ans == -1) printf("NO WAY\n"); else printf("ERROR 0\n"); }
    free(bitArr);
    return 0;
}

