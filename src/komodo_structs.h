/******************************************************************************
 * Copyright © 2014-2016 The SuperNET Developers.                             *
 *                                                                            *
 * See the AUTHORS, DEVELOPER-AGREEMENT and LICENSE files at                  *
 * the top-level directory of this distribution for the individual copyright  *
 * holder information and the developer policies on copyright and licensing.  *
 *                                                                            *
 * Unless otherwise agreed in a custom licensing agreement, no part of the    *
 * SuperNET software, including this file may be copied, modified, propagated *
 * or distributed except according to the terms contained in the LICENSE file *
 *                                                                            *
 * Removal or modification of this copyright notice is prohibited.            *
 *                                                                            *
 ******************************************************************************/

#include "uthash.h"
#include "utlist.h"

/*#ifdef WIN32
#define PACKED
#else
#define PACKED __attribute__((packed))
#endif*/

#define GENESIS_NBITS 0x1f00ffff
#define KOMODO_MINRATIFY ((height < 90000) ? 7 : 13)
#define KOMODO_MAXBLOCKS 5000000

#define KOMODO_EVENT_RATIFY 'P'
#define KOMODO_EVENT_NOTARIZED 'N'
#define KOMODO_EVENT_KMDHEIGHT 'K'
#define KOMODO_EVENT_REWIND 'B'
#define KOMODO_EVENT_PRICEFEED 'V'
#define KOMODO_EVENT_OPRETURN 'R'
#define KOMODO_OPRETURN_DEPOSIT 'D'
#define KOMODO_OPRETURN_ISSUED 'I' // assetchain
#define KOMODO_OPRETURN_WITHDRAW 'W' // assetchain
#define KOMODO_OPRETURN_REDEEMED 'X'

struct komodo_event_notarized { uint256 blockhash,desttxid; int32_t notarizedheight; char dest[16]; };
struct komodo_event_pubkeys { uint8_t num; uint8_t pubkeys[64][33]; };
struct komodo_event_opreturn { uint256 txid; uint64_t value; uint16_t vout,oplen; uint8_t opret[]; };
struct komodo_event_pricefeed { uint8_t num; uint32_t prices[35]; };

struct komodo_event
{
    struct komodo_event *related;
    uint16_t len;
    int32_t height;
    uint8_t type,reorged;
    char symbol[16];
    uint8_t space[];
};

struct pax_transaction
{
    UT_hash_handle hh;
    uint256 txid;
    uint64_t komodoshis,fiatoshis,validated;
    int32_t marked,height,otherheight,approved,didstats;
    uint16_t vout;
    char symbol[16],source[16],coinaddr[64]; uint8_t rmd160[20],shortflag;
};

struct knotary_entry { UT_hash_handle hh; uint8_t pubkey[33],notaryid; };
struct knotaries_entry { int32_t height,numnotaries; struct knotary_entry *Notaries; };
struct notarized_checkpoint { uint256 notarized_hash,notarized_desttxid; int32_t nHeight,notarized_height; };

struct komodo_state
{
    uint256 NOTARIZED_HASH,NOTARIZED_DESTTXID;
    int32_t SAVEDHEIGHT,CURRENT_HEIGHT,NOTARIZED_HEIGHT;
    uint32_t SAVEDTIMESTAMP;
    uint64_t deposited,issued,withdrawn,approved,redeemed;
    struct notarized_checkpoint *NPOINTS; int32_t NUM_NPOINTS;
    struct komodo_event **Komodo_events; int32_t Komodo_numevents;
    uint32_t RTbufs[64][3]; uint64_t RTmask;
};
