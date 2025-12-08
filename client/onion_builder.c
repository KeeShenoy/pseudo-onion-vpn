
// onion_builder.c
#include "onion_builder.h"
#include "../crypto/aes.h"
#include "../crypto/keys.h"
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

// Ports
#define PORT_NODE1 8001
#define PORT_NODE2 8002
#define PORT_SERVER 8003

// Build the final packet to send to Node1:
// [2 bytes port_Node2][2 bytes port_Server][L1_enc]
uint8_t* build_onion(const char *message, size_t *out_len) {

    size_t msg_len = strlen(message);
    size_t L3_len, L2_len, L1_len;

    // -------------------
    // LAYER 3 (for SERVER)  -> plaintext message encrypted for server
    // L3_enc = AES_CBC_Encrypt(message)
    // -------------------
    uint8_t *L3_enc = AES128_CBC_Encrypt((uint8_t*)message, msg_len, KEY_SERVER, &L3_len);

    // -------------------
    // LAYER 2 (for NODE2)  -> encrypt L3_enc for Node2
    // L2_enc = AES_CBC_Encrypt(L3_enc)
    // -------------------
    uint8_t *L2_enc = AES128_CBC_Encrypt(L3_enc, L3_len, KEY_NODE2, &L2_len);
    free(L3_enc);

    // -------------------
    // LAYER 1 (for NODE1)  -> encrypt L2_enc for Node1
    // L1_enc = AES_CBC_Encrypt(L2_enc)
    // -------------------
    uint8_t *L1_enc = AES128_CBC_Encrypt(L2_enc, L2_len, KEY_NODE1, &L1_len);
    free(L2_enc);

    // -------------------
    // Final packet: [port_node2(2)][port_server(2)][L1_enc]
    // -------------------
    *out_len = 2 + 2 + L1_len;
    uint8_t *packet = malloc(*out_len);

    uint16_t p_node2 = htons(PORT_NODE2);
    uint16_t p_server = htons(PORT_SERVER);

    memcpy(packet, &p_node2, 2);
    memcpy(packet + 2, &p_server, 2);
    memcpy(packet + 4, L1_enc, L1_len);

    free(L1_enc);
    return packet;
}

















/* // 1st onion_builder.c
#include "onion_builder.h"
#include "../crypto/aes.h"
#include "../crypto/keys.h"
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

// Ports
#define PORT_NODE1 8001
#define PORT_NODE2 8002
#define PORT_SERVER 8003

// Helper: prepend next hop port to buffer
static uint8_t* wrap_with_port(uint16_t port, const uint8_t *data, size_t data_len, size_t *out_len) {
    *out_len = data_len + 2;
    uint8_t *buf = malloc(*out_len);

    uint16_t p = htons(port);
    memcpy(buf, &p, 2);
    memcpy(buf + 2, data, data_len);

    return buf;
}

uint8_t* build_onion(const char *message, size_t *out_len) {

    size_t msg_len = strlen(message);
    size_t L3_len, L2_len, L1_len;

    // -------------------
    // LAYER 3 (for SERVER)
    // -------------------
    uint8_t *L3_enc = AES128_CBC_Encrypt((uint8_t*)message, msg_len, KEY_SERVER, &L3_len);

    uint8_t *L3_final = wrap_with_port(PORT_SERVER, L3_enc, L3_len, &L3_len);
    free(L3_enc);

    // -------------------
    // LAYER 2 (for NODE2)
    // -------------------
    uint8_t *L2_enc = AES128_CBC_Encrypt(L3_final, L3_len, KEY_NODE2, &L2_len);
    free(L3_final);

    uint8_t *L2_final = wrap_with_port(PORT_SERVER, L2_enc, L2_len, &L2_len);
    free(L2_enc);

    // -------------------
    // LAYER 1 (for NODE1)
    // -------------------
    uint8_t *L1_enc = AES128_CBC_Encrypt(L2_final, L2_len, KEY_NODE1, &L1_len);
    free(L2_final);

    uint8_t *L1_final = wrap_with_port(PORT_NODE2, L1_enc, L1_len, out_len);
    free(L1_enc);

    return L1_final;
}
*/
