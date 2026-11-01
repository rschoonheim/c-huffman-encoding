
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SYMBOLS 256

/* Zet op 0 om debug-uitvoer uit te schakelen */
#define DEBUG 1

#if DEBUG
#define DBG(...) fprintf(stderr, __VA_ARGS__)
#else
#define DBG(...) ((void)0)
#endif

typedef struct Node
{
    unsigned char symbol;
    unsigned long freq;
    struct Node *left, *right;
} Node;

Node* new_node(unsigned char symbol, unsigned long freq)
{
    Node* n = malloc(sizeof(Node));
    n->symbol = symbol;
    n->freq = freq;
    n->left = n->right = NULL;
    return n;
}

/* Eenvoudige min-heap op Node-pointers */
typedef struct
{
    Node** data;
    int size;
} Heap;

void heap_push(Heap* h, Node* n)
{
    int i = h->size++;
    h->data[i] = n;
    while (i > 0)
    {
        int p = (i - 1) / 2;
        if (h->data[p]->freq <= h->data[i]->freq) break;
        Node* tmp = h->data[p];
        h->data[p] = h->data[i];
        h->data[i] = tmp;
        i = p;
    }
}

Node* heap_pop(Heap* h)
{
    Node* top = h->data[0];
    h->data[0] = h->data[--h->size];
    int i = 0;
    while (1)
    {
        int l = 2 * i + 1, r = 2 * i + 2, s = i;
        if (l < h->size && h->data[l]->freq < h->data[s]->freq) s = l;
        if (r < h->size && h->data[r]->freq < h->data[s]->freq) s = r;
        if (s == i) break;
        Node* tmp = h->data[s];
        h->data[s] = h->data[i];
        h->data[i] = tmp;
        i = s;
    }
    return top;
}

/* Hulpfunctie: print een symbool leesbaar */
static void print_symbol(unsigned char c, char* buf)
{
    if (c >= 32 && c < 127) sprintf(buf, "'%c'", c);
    else if (c == '\n') sprintf(buf, "'\\n'");
    else if (c == '\t') sprintf(buf, "'\\t'");
    else if (c == '\r') sprintf(buf, "'\\r'");
    else sprintf(buf, "0x%02X", c);
}

void build_codes(Node* node, char* code, int depth,
                 char codes[MAX_SYMBOLS][MAX_SYMBOLS])
{
    if (!node->left && !node->right)
    {
        if (depth == 0) strcpy(codes[node->symbol], "0");
        else code[depth] = '\0', strcpy(codes[node->symbol], code);
        return;
    }
    if (node->left)
    {
        code[depth] = '0';
        build_codes(node->left, code, depth + 1, codes);
    }
    if (node->right)
    {
        code[depth] = '1';
        build_codes(node->right, code, depth + 1, codes);
    }
}

Node* build_tree(unsigned long freq[MAX_SYMBOLS])
{
    Heap h;
    h.data = malloc(sizeof(Node*) * (2 * MAX_SYMBOLS));
    h.size = 0;
    for (int i = 0; i < MAX_SYMBOLS; i++)
        if (freq[i] > 0)
            heap_push(&h, new_node((unsigned char)i, freq[i]));
    DBG("[debug] build_tree: %d unieke symbolen in heap\n", h.size);
    if (h.size == 0)
    {
        free(h.data);
        return NULL;
    }
    while (h.size > 1)
    {
        Node* a = heap_pop(&h);
        Node* b = heap_pop(&h);
        Node* parent = new_node(0, a->freq + b->freq);
        parent->left = a;
        parent->right = b;
        heap_push(&h, parent);
    }
    Node* root = heap_pop(&h);
    free(h.data);
    DBG("[debug] build_tree: boom klaar, totale freq (root) = %lu\n", root->freq);
    return root;
}

void free_tree(Node* n)
{
    if (!n) return;
    free_tree(n->left);
    free_tree(n->right);
    free(n);
}

void compress(const char* infile, const char* outfile)
{
    FILE* in = fopen(infile, "rb");
    if (!in)
    {
        fprintf(stderr, "Fout: kan '%s' niet openen\n", infile);
        exit(1);
    }
    FILE* out = fopen(outfile, "wb");
    if (!out)
    {
        fprintf(stderr, "Fout: kan '%s' niet openen\n", outfile);
        fclose(in);
        exit(1);
    }

    DBG("[debug] compress: input='%s', output='%s'\n", infile, outfile);

    /* 1. Frequenties tellen */
    unsigned long freq[MAX_SYMBOLS] = {0};
    unsigned long total = 0;
    int c;
    while ((c = fgetc(in)) != EOF)
    {
        freq[c]++;
        total++;
    }
    DBG("[debug] %lu bytes gelezen\n", total);

#if DEBUG
    DBG("[debug] frequentietabel:\n");
    for (int i = 0; i < MAX_SYMBOLS; i++)
    {
        if (freq[i] > 0)
        {
            char sbuf[8];
            print_symbol((unsigned char)i, sbuf);
            DBG("[debug]   %-5s freq=%lu\n", sbuf, freq[i]);
        }
    }
#endif

    /* 2. Boom bouwen */
    Node* root = build_tree(freq);
    if (!root)
    {
        DBG("[debug] lege input, alleen header geschreven\n");
        fwrite(freq, sizeof(freq), 1, out);
        fclose(in);
        fclose(out);
        return;
    }

    /* 3. Codes genereren */
    char codes[MAX_SYMBOLS][MAX_SYMBOLS] = {{0}};
    char buffer[MAX_SYMBOLS];
    build_codes(root, buffer, 0, codes);

#if DEBUG
    DBG("[debug] Huffman-codes:\n");
    for (int i = 0; i < MAX_SYMBOLS; i++)
    {
        if (freq[i] > 0)
        {
            char sbuf[8];
            print_symbol((unsigned char)i, sbuf);
            DBG("[debug]   %-5s len=%zu code=%s\n", sbuf, strlen(codes[i]), codes[i]);
        }
    }
#endif

    /* 4. Header (frequenties) + gecodeerde data schrijven */
    fwrite(freq, sizeof(freq), 1, out);
    rewind(in);
    unsigned char byte = 0;
    int bitcount = 0;
    unsigned long bits_written = 0;
    while ((c = fgetc(in)) != EOF)
    {
        for (char* p = codes[c]; *p; p++)
        {
            byte <<= 1;
            if (*p == '1') byte |= 1;
            bits_written++;
            if (++bitcount == 8)
            {
                fputc(byte, out);
                byte = 0;
                bitcount = 0;
            }
        }
    }
    if (bitcount > 0)
    {
        byte <<= (8 - bitcount);
        fputc(byte, out);
        DBG("[debug] laatste byte opgevuld met %d padding-bits\n", 8 - bitcount);
    }

    unsigned long payload_bytes = (bits_written + 7) / 8;
    unsigned long out_total = sizeof(freq) + payload_bytes;
    DBG("[debug] %lu bits geschreven (%lu payload-bytes)\n", bits_written, payload_bytes);
    DBG("[debug] header: %zu bytes, totaal output: %lu bytes\n", sizeof(freq), out_total);
    if (total > 0)
        DBG("[debug] ratio (payload/input): %.2f%%, gem. %.2f bits/symbool\n",
            100.0 * (double)payload_bytes / (double)total,
            (double)bits_written / (double)total);

    fclose(in);
    fclose(out);
    free_tree(root);
}

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "Gebruik: %s <input> <output>\n", argv[0]);
        return 1;
    }
    compress(argv[1], argv[2]);
    return 0;
}