////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
#ifndef Chess_hpp
#define Chess_hpp
#include <stdlib.h>
#include <string>
#include <list>
#include <map>
#include <set>

////////////////////////////////////////////////////////////////////////////////
//
#ifndef GameTypes_hpp
#define GameTypes_hpp
enum Side { None, White, Black };
#endif
enum Color { Light, Dark };
enum Piece { Empty, Pawn, Rook, Knight, Bishop, Queen, King };
enum Row { r1, r2, r3, r4, r5, r6, r7, r8, rMax };
enum Col { ca, cb, cc, cd, ce, cf, cg, ch, cMax };
enum CastleType { NoCastle, KingSide, QueenSide };

typedef Side Turn;

////////////////////////////////////////////////////////////////////////////////
//
class Square
{
public:
    Square()
        : color(Dark)
        , row(r1)
        , col(ca)
        , piece(Empty)
        , side(None)
        , attackWhite(0)
        , attackBlack(0)
    {
    }

    void setSquare(Color _color, Row _row, Col _col)
    {
        color = _color;
        row = _row;
        col = _col;
    }

    bool setPiece(Piece _piece, Side _side)
    {
        if (  ((_piece == Empty) && (_side == None))
           || ((_piece != Empty) && (_side != None))) {
            piece = _piece;
            side = _side;
            return true;
        }
        return false;
    }

    void clear()
    {
        piece = Empty;
        side = None;
    }

    void whoIs(Piece& p, Side& s) const { p = piece; s = side; }

    Piece getPiece() const { return piece; }
    Side getSide() const { return side; }
    bool isWhite() const { return side == White; }
    bool isBlack() const { return side == Black; }
    bool isOpen() const { return piece == Empty; }

    Color getColor() const { return color; }

    Row getRow() const { return row; }
    Col getCol() const { return col; }

    int getAttackWhite() const { return attackWhite; }
    int getAttackBlack() const { return attackBlack; }

    void incrAttackWhite() { ++attackWhite; }
    void incrAttackBlack() { ++attackBlack; }

    void decrAttackWhite() { --attackWhite; }
    void decrAttackBlack() { --attackBlack; }

    void clrAttackWhite() { attackWhite = 0; }
    void clrAttackBlack() { attackBlack = 0; }

    void clearAttacks() { clrAttackWhite(); clrAttackBlack(); } 

    bool operator==(const Square& rhs) const { return compare(rhs) == 0; }
    bool operator!=(const Square& rhs) const { return compare(rhs) != 0; }
    bool operator>(const Square& rhs) const { return compare(rhs) > 0; }
    bool operator>=(const Square& rhs) const { return compare(rhs) >= 0; }
    bool operator<(const Square& rhs) const { return compare(rhs) < 0; }
    bool operator<=(const Square& rhs) const { return compare(rhs) <= 0; }

    const char *toString() const
    {
        return toString(piece, side);
    }

    static const char *toString(Piece p, Side s)
    {
        switch (p)
        {
        case Pawn:   return (s == White) ? " Pw " : " Pb ";
        case Rook:   return (s == White) ? " Rw " : " Rb ";
        case Knight: return (s == White) ? " Nw " : " Nb ";
        case Bishop: return (s == White) ? " Bw " : " Bb ";
        case Queen:  return (s == White) ? " Qw " : " Qb ";
        case King:   return (s == White) ? " Kw " : " Kb ";
        default: return "    ";
        }
    }

    static const char *toString(Row r)
    {
        switch (r)
        {
        case r1: return "1";
        case r2: return "2";
        case r3: return "3";
        case r4: return "4";
        case r5: return "5";
        case r6: return "6";
        case r7: return "7";
        case r8: return "8";
        default: return "";
        }
    }

    static const char *toString(Col c)
    {
        switch (c)
        {
        case ca: return "a";
        case cb: return "b";
        case cc: return "c";
        case cd: return "d";
        case ce: return "e";
        case cf: return "f";
        case cg: return "g";
        case ch: return "h";
        default: return "";
        }
    }

private:
    int compare(const Square& rhs) const {
        if (row < rhs.row) { return -1; } else if (row > rhs.row) { return 1; }
        if (col < rhs.col) { return -1; } else if (col > rhs.col) { return 1; }
        if (piece < rhs.piece) { return -1; } else if (piece > rhs.piece) { return 1; }
        if (side < rhs.side) { return -1; } else if (side > rhs.side) { return 1; }
        return 0;
    }

private:
    Color color;
    Row row;
    Col col;
    Piece piece;
    Side side;
    int attackWhite;
    int attackBlack;
};

////////////////////////////////////////////////////////////////////////////////
//
class Move
{
public:
    Move(const Piece _piece = Empty,
         const Side _side = None,
         const Row _rF = r1, const Col _cF = ca,
         const Row _rT = r1, const Col _cT = ca,
         const bool _enpassant = false,
         const CastleType _castle = NoCastle)
        : piece(_piece)
        , side(_side)
        , rF(_rF)
        , cF(_cF)
        , rT(_rT)
        , cT(_cT)
        , enpassant(_enpassant)
        , castle(_castle)
    {
    }

    Piece getPiece() const { return piece; }
    Side getSide() const { return side; }

    bool isKing() const { return piece == King; }

    bool isWhite() const { return side == White; }
    bool isBlack() const { return side == Black; }
    bool isEnpassant() const { return enpassant; }
    bool isCastle() const { return castle != NoCastle; }
    bool isKingSide() const { return castle == KingSide; }
    bool isQueueSide() const { return castle == QueenSide; }

    // get the from position of this move
    void getFrom(Row& _rF, Col& _cF) const { _rF = rF; _cF = cF; }
    // get the to position of this move
    void getTo(Row& _rT, Col& _cT) const { _rT = rT; _cT = cT; }
    // get the en passant relative to this move
    void getEnpassantPosition(Row& _rP, Col& _cP) const { _rP = rF; _cP = cT; }
 
    bool operator==(const Move& rhs) const { return compare(rhs) == 0; }
    bool operator!=(const Move& rhs) const { return compare(rhs) != 0; }
    bool operator>(const Move& rhs) const { return compare(rhs) > 0; }
    bool operator>=(const Move& rhs) const { return compare(rhs) >= 0; }
    bool operator<(const Move& rhs) const { return compare(rhs) < 0; }
    bool operator<=(const Move& rhs) const { return compare(rhs) <= 0; }

    bool wasFirstPawnDoubleMove() const { return ((piece == Pawn) && ((rF > rT) ? (rF - rT == 2) : (rT - rF) == 2)); }

    Row rowF() const { return rF; }
    Row rowT() const { return rT; }
    Col colF() const { return cF; }
    Col colT() const { return cT; }

    void toStringPiece(std::string& str) const
    {
        str += Square::toString(piece, side);
        str += Square::toString(cF);
        str += Square::toString(rF);
    }

    void toStringMove(std::string& str) const
    {
        str += Square::toString(piece, side);
        str += Square::toString(cF);
        str += Square::toString(rF);
        str += "->";
        str += Square::toString(cT);
        str += Square::toString(rT);
    }

private:
    int compare(const Move& rhs) const {
        if (rF < rhs.rF) { return -1; } else if (rF > rhs.rF) { return 1; }
        if (cF < rhs.cF) { return -1; } else if (cF > rhs.cF) { return 1; }
        if (rT < rhs.rT) { return -1; } else if (rT > rhs.rT) { return 1; }
        if (cT < rhs.cT) { return -1; } else if (cT > rhs.cT) { return 1; }
        return 0;
    }

private:
    Piece piece;
    Side side;
    Row rF;
    Col cF;
    Row rT;
    Col cT;
    bool enpassant;
    CastleType castle;
};

////////////////////////////////////////////////////////////////////////////////
//
typedef std::set<Move> Moves;
typedef Moves::iterator MovesItr;
typedef Moves::const_iterator MovesCItr;

typedef Moves Pieces;
typedef MovesItr PiecesItr;
typedef MovesCItr PiecesCItr;

////////////////////////////////////////////////////////////////////////////////
//
class Castle
{
public:
    Castle(Side _side)
        : side(_side)
        , rookQ(true)
        , rookK(true)
        , king(true)
    {
    }

    bool castle() const { return (kingSide() || queenSide()); }
    bool kingSide() const { return (king && rookK); }
    bool queenSide() const { return (king && rookQ); }

    void checkMove(Row r, Col c, Piece piece, Side s)
    {
        if (side != s) { return; }
        switch (piece)
        {
        case King:
            king = false;
            return;
        case Rook:
            if ((side == White) && (r == r1)) {
                if (c == ca) { rookQ = false; }
                if (c == ch) { rookK = false; }
                return;
            }
            if ((side == Black) && (r == r8)) {
                if (c == ca) { rookQ = false; }
                if (c == ch) { rookK = false; }
                return;
            }
            return;
        default:
            return;
        }
    }

private:
    Side side;
    bool rookQ;
    bool rookK;
    bool king;
};

////////////////////////////////////////////////////////////////////////////////
//
class Board
{
public:
    Board(unsigned _seed = 0, Turn _turn = White, bool pieces = false)
        : EN_PASSANT(true)
        , seed(_seed)
        , turn(_turn)
        , white(White)
        , black(Black)
        , checkAttacker(NULL)
        , whiteCheck(false)
        , blackCheck(false)
        , promotion(false)
        , castle(false)
        , enpassant(false)
    {
        init(pieces);
    }

    Turn getTurn() const { return turn; }

    bool whiteCastle() const { return white.castle(); }
    bool whiteKingSide() const { return white.kingSide(); }
    bool whiteQueenSide() const { return white.queenSide(); }

    bool blackCastle() const { return black.castle(); }
    bool blackKingSide() const { return black.kingSide(); }
    bool blackQueenSide() const { return black.queenSide(); }

    bool move(bool& checkMate, bool& draw)
    {
        Turn player = getTurn();
        // get black moves and attacks
        Moves movesWhite;
        Moves attacksWhite;
        // get black moves and attacks
        Moves movesBlack;
        Moves attacksBlack;
        // see if my king is in check on this turn
        clearAttacks();
        // remove any king moves into check
        if (player == White) {
            // set black attacks
            blackMoves(movesBlack, attacksBlack);
            setAttacks(attacksBlack, Black);
            // get white moves
            whiteMoves(movesWhite, attacksWhite);
            removeCheckMoves(movesWhite);
        } else {
            // set white attacks
            whiteMoves(movesWhite, attacksWhite);
            setAttacks(attacksWhite, White);
            // get black moves
            blackMoves(movesBlack, attacksBlack);
            removeCheckMoves(movesBlack);
        }
        // find possible moves if in check
        movesCheck.clear();
        if (getCheck(player)) {
            if (player == White) {
                checkMoves(movesWhite, movesCheck);
            } else {
                checkMoves(movesBlack, movesCheck);
            }
            if (movesCheck.empty()) {
                checkMate = true;
                draw = false;
                return false;
            }
        }
        // pick our set of moves to chose from
        const Moves& moves = (player == White)
                           ? (movesCheck.empty() ? movesWhite : movesCheck)
                           : (movesCheck.empty() ? movesBlack : movesCheck);
        if (moves.empty()) {
            checkMate = false;
            draw = true;
            return false;
        }
        const Move *move = selectMove(moves);
        play(*move);
        checkMate = false;
        draw = false;
        turn = (player == White) ? Black : White;
        return true;
    }

    const Move *selectMove(const Moves& moves) const
    {
        return randomMove(moves);
    }

    const Move *randomMove(const Moves& moves) const
    {
        if (moves.empty()) { return NULL; }
        unsigned *pseed = (unsigned *)&seed;
        unsigned idx = rand_r(pseed) % moves.size();
        MovesCItr itr = moves.begin();
        for (unsigned i = 0; i < idx; ++i, ++itr) {}
        return &(*itr);
    }

    // move piece from rF, cF, to rT, cT
    void play(const Move& move)
    {
        const Row rF = move.rowF();
        const Col cF = move.colF();
        const Row rT = move.rowT();
        const Col cT = move.colT();
        // get from piece
        Piece pF; Side sF;
        whoIs(rF, cF, pF, sF);
        // get to piece
        Piece pT; Side sT;
        whoIs(rT, cT, pT, sT);
        // keep track of castling
        if (sF == White) {
            white.checkMove(rF, cF, pF, sF);
        } else {
            black.checkMove(rF, cF, pF, sF);
        }
        // clear our from square
        clearSquare(rF, cF);
        // set our to square
        setBoardPiece(rT, cT, pF, sF);
        // update our side piece
        updSidePiece(rF, cF, rT, cT, pF, sF);
        // check on enpassant
        doEnpassant(move);
        // check on castling
        doCastle(move);
        // was a piece captured 
        if ((pT != Empty) && (sT != None)) {
            remSidePiece(rT, cT, pT, sT);
        }
        // remember this move
        gameMoves.push_back(move);
    }

    void doEnpassant(const Move& move)
    {
        if (!move.isEnpassant()) { return; }
        Row rT = move.rowF();
        Col cT = move.colT();
        // get to piece
        Piece pT; Side sT;
        whoIs(rT, cT, pT, sT);
        clearSquare(rT, cT);
        remSidePiece(rT, cT, pT, sT);
        // set enpassant flag
        enpassant = true;
    }

    void doCastle(const Move& move)
    {
        if (!move.isCastle()) { return; }
        // which Rook to move?
        Piece pF = Rook;
        Side sF = move.isWhite() ? White : Black;
        Row rF = move.isWhite() ? r1 : r8;
        Col cF = move.isKingSide() ? ch : ca;
        Row rT = move.isWhite() ? r1 : r8;
        Col cT = move.isKingSide() ? cf : cd;
        // clear our from square
        clearSquare(rF, cF);
        // set our to square
        setBoardPiece(rT, cT, pF, sF);
        // update our side piece
        updSidePiece(rF, cF, rT, cT, pF, sF);
        // set castle flag
        castle = true;
    }

    const Square& getSquare(const Row r, const Col c) const { return board[r][c]; }
    void clearSquare(const Row r, const Col c) { board[r][c].clear(); }

    void whoIs(const Row r, const Col c, Piece& p, Side& s) const { board[r][c].whoIs(p, s); }
    bool isOpen(const Row r, const Col c) const { return board[r][c].isOpen(); }
    bool isWhite(const Row r, const Col c) const { return board[r][c].isWhite(); }
    bool isBlack(const Row r, const Col c) const { return board[r][c].isBlack(); }

    void getAllMovesTo(const Moves& moves, Row rT, Col cT, Moves& movesChk) const
    {
        MovesCItr itr = moves.begin();
        for (; itr != moves.end(); ++itr) {
            const Move& move = *itr;
            if ((move.rowT() == rT) && (move.colT() == cT)) {
                if (move.getPiece() == King) {
                    // can only move king to this position if it is not under attack by other side
                    if (  ((move.getSide() == White) && (board[rT][cT].getAttackBlack() == 0))
                       || ((move.getSide() == Black) && (board[rT][cT].getAttackWhite() == 0))) {
                        movesChk.insert(move);
                    }
                } else {
                    movesChk.insert(move);
                }
            }
        }
    }

    void getAllKingMoves(const Moves& moves, Moves& movesChk) const
    {
        MovesCItr itr = moves.begin();
        for (; itr != moves.end(); ++itr) {
            const Move& move = *itr;
            if (move.getPiece() == King) {
                movesChk.insert(move);
            }
        }
    }

    void removeCheckMoves(Moves& moves) const
    {
        // find in check moves
        std::list<Move> inCheck;
        MovesCItr itr = moves.begin();
        for (; itr != moves.end(); ++itr) {
            const Move& move = *itr;
            if (move.getPiece() == King) {
                Row rT = move.rowT(); Col cT = move.colT();
                if (  ((move.getSide() == White) && (board[rT][cT].getAttackBlack()))
                   || ((move.getSide() == Black) && (board[rT][cT].getAttackWhite()))) {
                    inCheck.push_back(move);
                }
            }
        }
        // remove in check moves
        std::list<Move>::iterator itr2 = inCheck.begin();
        for (; itr2 != inCheck.end(); ++itr2) {
            moves.erase(*itr2);
        }
    }

    void checkBlockRookMoves(const Moves& moves, Moves& movesChk) const
    {
        if (!checkAttacker) { return; }
        // rook is at rF, cF
        Row rF = checkAttacker->rowF();
        Col cF = checkAttacker->colF();
        // king is at rT, cT
        Row rT = checkAttacker->rowT();
        Col cT = checkAttacker->colT();
        /*
         *       c   d   e   f   g
         *     *---*---*---*---*---*
         *   6 |   |   | R |   |   |
         *     *---*---*---*---*---*
         *   5 |   |   | x |   |   |
         *     *---*---*---*---*---*
         *   4 | R | x | K | x | R | rF == rT
         *     *---*---*---*---*---*
         *   3 |   |   | x |   |   |
         *     *---*---*---*---*---*
         *   2 |   |   | R |   |   |
         *     *---*---*---*---*---*
         *            cF == cT
         */
        if (rF == rT) {
            // row attack
            if (cF < cT) {
                for (int c = cF + 1; c < cT; ++c) {
                    getAllMovesTo(moves, rF, (Col)c, movesChk);
                }
            } else if (cF > cT) {
                for (int c = cF - 1; c > cT; --c) {
                    getAllMovesTo(moves, rF, (Col)c, movesChk);
                }
            }
        } else if (cF == cT) {
            // column attack
            if (rF < rT) {
                for (int r = rF + 1; r < rT; ++r) {
                    getAllMovesTo(moves, (Row)r, cF, movesChk);
                }
            } else if (rF > rT) {
                for (int r = rF - 1; r > rT; --r) {
                    getAllMovesTo(moves, (Row)r, cF, movesChk);
                }
            }
        }
    }

    void checkBlockBishopMoves(const Moves& moves, Moves& movesChk) const
    {
        if (!checkAttacker) { return; }
        // bishop is at rF, cF
        Row rF = checkAttacker->rowF();
        Col cF = checkAttacker->colF();
        // king is at rT, cT
        Row rT = checkAttacker->rowT();
        Col cT = checkAttacker->colT();
        /*
         *       c   d   e   f   g
         *     *---*---*---*---*---*
         *   6 | B |   |   |   | B |
         *     *---*---*---*---*---*
         *   5 |   | x |   | x |   | rF > rT
         *     *---*---*---*---*---*
         *   4 |   |   | K |   |   |
         *     *---*---*---*---*---*
         *   3 |   | x |   | x |   | rF < rT
         *     *---*---*---*---*---*
         *   2 | B |   |   |   | B |
         *     *---*---*---*---*---*
         *      cF < cT    cF > cT
         */
        if (rF < rT) {
            int r = rF + 1;
            if (cF < cT) {
                int c = cF + 1;
                for (; r < rT && c < cT; ++r, ++c) {
                    getAllMovesTo(moves, (Row)r, (Col)c, movesChk);
                }
            } else if (cF > cT) {
                int c = cF - 1;
                for (; r < rT && c > cT; ++r, --c) {
                    getAllMovesTo(moves, (Row)r, (Col)c, movesChk);
                }
            }
        }
        else if (rF > rT) {
            int r = rF - 1;
            if (cF < cT) {
                int c = cF + 1;
                for (; r > rT && c < cT; --r, ++c) {
                    getAllMovesTo(moves, (Row)r, (Col)c, movesChk);
                }
            } else if (cF > cT) {
                int c = cF - 1;
                for (; r > rT && c > cT; --r, --c) {
                    getAllMovesTo(moves, (Row)r, (Col)c, movesChk);
                }
            }
        }
    }

    // all possible king moves while my king is in check
    void checkMoves(const Moves& moves, Moves& movesChk) const
    {
        if (!checkAttacker) { return; }
        Piece piece = checkAttacker->getPiece();
        // attacker is at rF, cF
        Row rF = checkAttacker->rowF();
        Col cF = checkAttacker->colF();
        // all moves that can kill the attacker
        getAllMovesTo(moves, rF, cF, movesChk);
        // all moves that can block the attacker
        switch (piece)
        {
        case Empty:
            break;
        case Pawn:
            // not possible to block pawn check attacks
            break;
        case Rook:
            checkBlockRookMoves(moves, movesChk);
            break;
        case Knight:
            // not possible to block knight check attacks
            break;
        case Bishop:
            checkBlockBishopMoves(moves, movesChk);
            break;
        case Queen:
            checkBlockRookMoves(moves, movesChk);
            checkBlockBishopMoves(moves, movesChk);
            break;
        case King:
            // King can never place another King in check
            break;
        }
        // get all king moves to squares not in check
        getAllKingMoves(moves, movesChk);
    }

    void whiteMoves(Moves& moves, Moves& attacks) const
    {
        sideMoves(whitePieces, moves, attacks);
    }

    void blackMoves(Moves& moves, Moves& attacks) const
    {
        sideMoves(blackPieces, moves, attacks);
    }

    void sideMoves(const Moves& pieces, Moves& moves, Moves& attacks) const
    {
        moves.clear();
        attacks.clear();
        MovesCItr itr = pieces.begin();
        for (; itr != pieces.end(); ++itr) {
            const Move& piece = *itr;
            switch (piece.getPiece())
            {
            case Empty: break;
            case Pawn: pawnMoves(piece, moves, attacks); break;
            case Rook: rookMoves(piece, moves, attacks); break;
            case Knight: knightMoves(piece, moves, attacks); break;
            case Bishop: bishopMoves(piece, moves, attacks); break;
            case Queen: queenMoves(piece, moves, attacks); break;
            case King: kingMoves(piece, moves, attacks); break;
            }
        }
    }

    void pawnMoves(const Move& piece, Moves& moves, Moves& attacks) const
    {
        if (piece.isWhite()) {
            pawnWhiteMoves(piece, moves, attacks);
        } else if (piece.isBlack()) {
            pawnBlackMoves(piece, moves, attacks);
        }
    }

    void pawnWhiteMoves(const Move& piece, Moves& moves, Moves& attacks) const
    {
        Row rF; Col cF;
        piece.getFrom(rF, cF);
        if (rF == r8) { return; }
        Row rT = (Row)(rF + 1);
        Col cT = cF;
        // can move one ahead
        if (isOpen(rT, cT)) {
            insert(moves, Move(Pawn, White, rF, cF, rT, cT));
        }
        // check left
        cT = (Col)(cF - 1);
        if (cF != ca) {
            // can always attack left
            insert(attacks, Move(Pawn, White, rF, cF, rT, cT));
            if (isBlack(rT, cT)) {
                // can move also
                insert(moves, Move(Pawn, White, rF, cF, rT, cT));
            }
        }
        // check right
        cT = (Col)(cF + 1);
        if (cF != ch) {
            // can always attack right
            insert(attacks, Move(Pawn, White, rF, cF, rT, cT));
            if (isBlack(rT, cT)) {
                // can move also
                insert(moves, Move(Pawn, White, rF, cF, rT, cT));
            }
        }
        // can move two on first move
        if (firstPawnMove(rF, White)) {
            Row rT = r4;
            Col cT = cF;
            if (isOpen(r3, cT) && isOpen(r4, cT)) {
                insert(moves, Move(Pawn, White, rF, cF, rT, cT));
            }
        }
        // handle En passant for White
        if (1)
        {
            Row rT;
            Col cT;
            if (lastMoveCanEnpassant(rF, cF, White, rT, cT)) {
                insert(moves, Move(Pawn, White, rF, cF, rT, cT, EN_PASSANT));
            }
        }
    }

    void pawnBlackMoves(const Move& piece, Moves& moves, Moves& attacks) const
    {
        Row rF; Col cF;
        piece.getFrom(rF, cF);
        if (rF == r1) { return; }
        Row rT = (Row)(rF - 1);
        Col cT = cF;
        // can move one ahead
        if (isOpen(rT, cT)) {
            insert(moves, Move(Pawn, Black, rF, cF, rT, cT));
        }
        // check left
        cT = (Col)(cF + 1);
        if (cF != ch) {
            // can attack left
            insert(attacks, Move(Pawn, Black, rF, cF, rT, cT));
            if (isWhite(rT, cT)) {
                // can move also
                insert(moves, Move(Pawn, Black, rF, cF, rT, cT));
            }
        }
        // check right
        cT = (Col)(cF - 1);
        if (cF != ca) {
            // can attack right
            insert(attacks, Move(Pawn, Black, rF, cF, rT, cT));
            if (isWhite(rT, cT)) {
                // can move also
                insert(moves, Move(Pawn, Black, rF, cF, rT, cT));
            }
        }
        // can move two on first move
        if (firstPawnMove(rF, Black)) {
            Row rT = r5;
            Col cT = cF;
            if (isOpen(r6, cT) && isOpen(r5, cT)) {
                insert(moves, Move(Pawn, Black, rF, cF, rT, cT));
            }
        }
        // handle En passant for Black
        if (1)
        {
            Row rT;
            Col cT;
            if (lastMoveCanEnpassant(rF, cF, Black, rT, cT)) {
                insert(moves, Move(Pawn, Black, rF, cF, rT, cT, EN_PASSANT));
            }
        }
    }

    void knightMoves(const Move& piece, Moves& moves, Moves& attacks) const
    {
        Row rF; Col cF;
        piece.getFrom(rF, cF);
        // forward
        knightMove(piece, moves, attacks, rF, cF, rF + 2, cF + 1);
        knightMove(piece, moves, attacks, rF, cF, rF + 2, cF - 1);
        knightMove(piece, moves, attacks, rF, cF, rF + 1, cF + 2);
        knightMove(piece, moves, attacks, rF, cF, rF + 1, cF - 2);
        // back
        knightMove(piece, moves, attacks, rF, cF, rF - 2, cF + 1);
        knightMove(piece, moves, attacks, rF, cF, rF - 2, cF - 1);
        knightMove(piece, moves, attacks, rF, cF, rF - 1, cF + 2);
        knightMove(piece, moves, attacks, rF, cF, rF - 1, cF - 2);
    }

    void knightMove(const Move& piece, Moves& moves, Moves& attacks, Row rF, Col cF, int _rT, int _cT) const
    {
        if (offBoard(_rT, _cT)) { return; }
        Row rT = (Row)_rT; Col cT = (Col)_cT;
        // can always attack
        insert(attacks, Move(Knight, piece.getSide(), rF, cF, rT, cT));
        if (canKnightMove(piece, rT, cT)) {
            // can move also
            insert(moves, Move(Knight, piece.getSide(), rF, cF, rT, cT));
        }
    }

    bool canKnightMove(const Move& piece, Row rT, Col cT) const
    {
        if (  isOpen(rT, cT)
           || (piece.isWhite() && isBlack(rT, cT)) 
           || (piece.isBlack() && isWhite(rT, cT))) {
            return true;
        }
        return false;
    }

    bool offBoard(int rT, int cT) const
    {
        if (rT < r1 || rT > r8) { return true; }
        if (cT < ca || cT > ch) { return true; }
        return false;
    }

    void rookMoves(const Move& piece, Moves& moves, Moves& attacks) const
    {
        moveN(piece, moves, attacks, 7);
        moveE(piece, moves, attacks, 7);
        moveS(piece, moves, attacks, 7);
        moveW(piece, moves, attacks, 7);
    }

    void bishopMoves(const Move& piece, Moves& moves, Moves& attacks) const
    {
        moveNE(piece, moves, attacks, 7);
        moveSE(piece, moves, attacks, 7);
        moveSW(piece, moves, attacks, 7);
        moveNW(piece, moves, attacks, 7);
    }

    void queenMoves(const Move& piece, Moves& moves, Moves& attacks) const
    {
        rookMoves(piece, moves, attacks);
        bishopMoves(piece, moves, attacks);
    }

    void kingMoves(const Move& piece, Moves& moves, Moves& attacks) const
    {
        moveN(piece, moves, attacks, 1);
        moveE(piece, moves, attacks, 1);
        moveS(piece, moves, attacks, 1);
        moveW(piece, moves, attacks, 1);
        moveNE(piece, moves, attacks, 1);
        moveSE(piece, moves, attacks, 1);
        moveSW(piece, moves, attacks, 1);
        moveNW(piece, moves, attacks, 1);
        moveCastle(piece, moves);
    }

    void moveCastle(const Move& piece, Moves& moves) const
    {
        Side side = piece.getSide();
        if (side == White) {
/*
    *----*----*----*----*----*----*----*----*
  1 | Rw |    |    |    | Kw |    |    | Rw |  1
    *----*----*----*----*----*----*----*----*
       a    b    c    d    e    f    g    h  
*/
            if (whiteCheck) { return; }
            if (white.kingSide()) {
                if (   board[r1][cf].isOpen()
                   &&  board[r1][cg].isOpen()
                   && !board[r1][cf].getAttackBlack()
                   && !board[r1][cg].getAttackBlack()) {
                    insert(moves, Move(King, White, r1, ce, r1, cg, !EN_PASSANT, KingSide));
                }
            }
            if (white.queenSide()) {
                if (   board[r1][cb].isOpen()
                   &&  board[r1][cc].isOpen()
                   &&  board[r1][cd].isOpen()
                   && !board[r1][cc].getAttackBlack()
                   && !board[r1][cd].getAttackBlack()) {
                    insert(moves, Move(King, White, r1, ce, r1, cc, !EN_PASSANT, QueenSide));
                }
            } 
        } else {
/*
    *----*----*----*----*----*----*----*----*
  8 | Rb |    |    |    | Kb |    |    | Rb |  8
    *----*----*----*----*----*----*----*----*
       a    b    c    d    e    f    g    h  
*/
            if (blackCheck) { return; }
            if (black.kingSide()) {
                if (   board[r8][cf].isOpen() 
                   &&  board[r8][cg].isOpen() 
                   && !board[r8][cf].getAttackBlack()
                   && !board[r8][cg].getAttackBlack()) {
                    insert(moves, Move(King, Black, r8, ce, r8, cg, !EN_PASSANT, KingSide));
                }
            }
            if (black.queenSide()) {
                if (   board[r8][cb].isOpen()
                   &&  board[r8][cc].isOpen()
                   &&  board[r8][cd].isOpen()
                   && !board[r8][cc].getAttackBlack()
                   && !board[r8][cd].getAttackBlack()) {
                    insert(moves, Move(King, Black, r8, ce, r8, cc, !EN_PASSANT, QueenSide));
                }
            }
        }
    }

    // move in the North direction r1 -> r8
    void moveN(const Move& piece, Moves& moves, Moves& attacks, int dist) const
    {
        Row rF = piece.rowF(); Col cF = piece.colF();
        int _rT = rF; int _cT = cF;
        while (!offBoard(++_rT, _cT) && (--dist >= 0)) {
            Row rT = (Row)_rT; Col cT = (Col)_cT;
            if (!addMove(piece, moves, attacks, rF, cF, rT, cT)) {
                return;
            }
        }
    }

    // move in the NorthEast direction r1 -> r8, ca -> ch
    void moveNE(const Move& piece, Moves& moves, Moves& attacks, int dist) const
    {
        Row rF = piece.rowF(); Col cF = piece.colF();
        int _rT = rF; int _cT = cF;
        while (!offBoard(++_rT, ++_cT) && (--dist >= 0)) {
            Row rT = (Row)_rT; Col cT = (Col)_cT;
            if (!addMove(piece, moves, attacks, rF, cF, rT, cT)) {
                return;
            }
        }
    }

    // move in the East direction ca -> ch
    void moveE(const Move& piece, Moves& moves, Moves& attacks, int dist) const
    {
        Row rF = piece.rowF(); Col cF = piece.colF();
        int _rT = rF; int _cT = cF;
        while (!offBoard(_rT, ++_cT) && (--dist >= 0)) {
            Row rT = (Row)_rT; Col cT = (Col)_cT;
            if (!addMove(piece, moves, attacks, rF, cF, rT, cT)) {
                return;
            }
        }
    }

    // move in the South direction r8 -> r1, ca -> ch
    void moveSE(const Move& piece, Moves& moves, Moves& attacks, int dist) const
    {
        Row rF = piece.rowF(); Col cF = piece.colF();
        int _rT = rF; int _cT = cF;
        while (!offBoard(--_rT, ++_cT) && (--dist >= 0)) {
            Row rT = (Row)_rT; Col cT = (Col)_cT;
            if (!addMove(piece, moves, attacks, rF, cF, rT, cT)) {
                return;
            }
        }
    }

    // move in the South direction r8 -> r1
    void moveS(const Move& piece, Moves& moves, Moves& attacks, int dist) const
    {
        Row rF = piece.rowF(); Col cF = piece.colF();
        int _rT = rF; int _cT = cF;
        while (!offBoard(--_rT, _cT) && (--dist >= 0)) {
            Row rT = (Row)_rT; Col cT = (Col)_cT;
            if (!addMove(piece, moves, attacks, rF, cF, rT, cT)) {
                return;
            }
        }
    }

    // move in the SouthWest direction r8 -> r1, ch -> ca
    void moveSW(const Move& piece, Moves& moves, Moves& attacks, int dist) const
    {
        Row rF = piece.rowF(); Col cF = piece.colF();
        int _rT = rF; int _cT = cF;
        while (!offBoard(--_rT, --_cT) && (--dist >= 0)) {
            Row rT = (Row)_rT; Col cT = (Col)_cT;
            if (!addMove(piece, moves, attacks, rF, cF, rT, cT)) {
                return;
            }
        }
    }

    // move in the West direction ch -> ca
    void moveW(const Move& piece, Moves& moves, Moves& attacks, int dist) const
    {
        Row rF = piece.rowF(); Col cF = piece.colF();
        int _rT = rF; int _cT = cF;
        while (!offBoard(_rT, --_cT) && (--dist >= 0)) {
            Row rT = (Row)_rT; Col cT = (Col)_cT;
            if (!addMove(piece, moves, attacks, rF, cF, rT, cT)) {
                return;
            }
        }
    }

    // move in the NorthWest direction r1 -> r8, ch -> ca
    void moveNW(const Move& piece, Moves& moves, Moves& attacks, int dist) const
    {
        Row rF = piece.rowF(); Col cF = piece.colF();
        int _rT = rF; int _cT = cF;
        while (!offBoard(++_rT, --_cT) && (--dist >= 0)) {
            Row rT = (Row)_rT; Col cT = (Col)_cT;
            if (!addMove(piece, moves, attacks, rF, cF, rT, cT)) {
                return;
            }
        }
    }

    bool canMove(const Move& piece, Row rT, Col cT, bool& more) const
    {
        if (isOpen(rT, cT)) { more = true; return true; }
        if (  (piece.isWhite() && isBlack(rT, cT)) 
           || (piece.isBlack() && isWhite(rT, cT))) {
            more = false;
            return true;
        }
        return false;
    }

    bool addMove(const Move& piece, Moves& moves, Moves& attacks, Row rF, Col cF, Row rT, Col cT) const
    {
        // can always attack
        insert(attacks, Move(piece.getPiece(), piece.getSide(), rF, cF, rT, cT));
        bool more;
        if (canMove(piece, rT, cT, more)) {
            // can move also
            insert(moves, Move(piece.getPiece(), piece.getSide(), rF, cF, rT, cT));
            return more;
        }
        return false;
    }

    void init(bool pieces)
    {
        initSquares();
        if (pieces) {
            initPieces();
        }
    }

    void initSquares()
    {
        for (int r = r1; r <= r8; ++r) {
            Color _color = ((r % 2) == 0) ? Dark : Light;
            for (int c = ca; c <= ch; ++c) {
                board[r][c].setSquare(_color, (Row)r, (Col)c);
                _color = (_color == Light) ? Dark : Light;
            }
        }
    }

    void initPieces()
    {
        initWhitePieces();
        initBlackPieces();
    }

    void initWhitePieces()
    {
        initPawns(r2, White);
        initOther(r1, White);
    }

    void initBlackPieces()
    {
        initPawns(r7, Black);
        initOther(r8, Black);
    }

    void initPawns(Row r, Side side)
    {
        for (int c = ca; c <= ch; ++c) {
            setBoardPiece(r, (Col)c, Pawn, side, true);
        }
    }

    void initOther(Row r, Side side)
    {
        setBoardPiece(r, ca, Rook, side, true);
        setBoardPiece(r, cb, Knight, side, true);
        setBoardPiece(r, cc, Bishop, side, true);
        setBoardPiece(r, cd, Queen, side, true);
        setBoardPiece(r, ce, King, side, true);
        setBoardPiece(r, cf, Bishop, side, true);
        setBoardPiece(r, cg, Knight, side, true);
        setBoardPiece(r, ch, Rook, side, true);
    }

    void setBoardPiece(Row r, Col c, Piece piece, Side side, bool addSide = false)
    {
        if ((side == None) || (piece == Empty)) { return; }
        board[r][c].setPiece(piece, side);
        if (addSide) {
            setSidePiece(r, c, piece, side);
        }
    }

    void setSidePiece(Row r, Col c, Piece piece, Side side)
    {
        if ((side == None) || (piece == Empty)) { return; }
        Pieces& pieces = (side == White) ? whitePieces : blackPieces;
        insert(pieces, Move(piece, side, r, c, r, c));
    }

    void updSidePiece(Row rF, Col cF, Row rT, Col cT, Piece piece, Side side)
    {
        if ((side == None) || (piece == Empty)) { return; }
        Pieces& pieces = (side == White) ? whitePieces : blackPieces;
        update(pieces, Move(piece, side, rF, cF, rF, cF), Move(promote(piece, side, rT), side, rT, cT, rT, cT));
    }

    void remSidePiece(Row r, Col c, Piece piece, Side side)
    {
        if ((side == None) || (piece == Empty)) { return; }
        Pieces& pieces = (side == White) ? whitePieces : blackPieces;
        Pieces& captured = (side == White) ? whiteCapturedPieces : blackCapturedPieces;
        remove(pieces, Move(piece, side, r, c, r, c));
        insert(captured, Move(piece, side, r, c, r, c));
    }

    bool firstPawnMove(Row r, Side s) const
    {
        if (r == r2 && s == White) {
            return true;
        } else if (r == r7 && s == Black) {
            return true;
        } else {
            return false;
        }
    }

    Piece promote(Piece piece, Side side, Row rT) const
    {
        bool yes = (  (piece == Pawn)
                   && (  ((side == White) && (rT == r8))
                      || ((side == Black) && (rT == r1))));
        return yes ? Queen : piece;
    }

    const Pieces& getMovesCheck() const { return movesCheck; }

    bool isValidRow(int r) const { return (r >= r1) && (r <= r8); }
    bool isValidCol(int c) const { return (c >= ca) && (c <= ch); }

    bool hasMoves() const { return !gameMoves.empty(); }

    bool lastMoveCanEnpassant(Row rF, Col cF, Side side, Row& rT, Col& cT) const
    {
        // only white pawns on the 5th rank can en passant
        if ((side == White) && (rF != r5)) { return false; }
        // only black pawns on the 4th rank can en passant
        if ((side == Black) && (rF != r4)) { return false; }
        if (!hasMoves()) { return false; }
        const Move& lastMove = *gameMoves.rbegin();
        if (!lastMove.wasFirstPawnDoubleMove()) { return false; }
        if (rF != lastMove.rowT()) { return false; }
        if (  (cF == (lastMove.colT() - 1))
           || (cF == (lastMove.colT() + 1))) {
            rT = (side == White) ? r6 : r3;
            cT = lastMove.colT();
            return true;
        }
        return false;
    }

    void toStringAttacks(Side side, std::string& str) const
    {
        str.clear();
        str += "       a    b    c    d    e    f    g    h  \n";
        str += "    *----*----*----*----*----*----*----*----*\n";
        for (int r = r8; r >= r1; --r) {
            char buf[256];
            const char *fmt = " %2d | %2d | %2d | %2d | %2d | %2d | %2d | %2d | %2d | %2d\n";
            snprintf(buf, sizeof(buf), fmt,
                     r + 1,
                     (side == White) ? board[r][ca].getAttackWhite() : board[r][ca].getAttackBlack(),
                     (side == White) ? board[r][cb].getAttackWhite() : board[r][cb].getAttackBlack(),
                     (side == White) ? board[r][cc].getAttackWhite() : board[r][cc].getAttackBlack(),
                     (side == White) ? board[r][cd].getAttackWhite() : board[r][cd].getAttackBlack(),
                     (side == White) ? board[r][ce].getAttackWhite() : board[r][ce].getAttackBlack(),
                     (side == White) ? board[r][cf].getAttackWhite() : board[r][cf].getAttackBlack(),
                     (side == White) ? board[r][cg].getAttackWhite() : board[r][cg].getAttackBlack(),
                     (side == White) ? board[r][ch].getAttackWhite() : board[r][ch].getAttackBlack(),
                     r + 1);
            str += buf;
            str += "    *----*----*----*----*----*----*----*----*\n";
        }
        str += "       a    b    c    d    e    f    g    h  \n";
    }

    void toString(std::string& str) const
    {
        str.clear();
        str += "       a    b    c    d    e    f    g    h  \n";
        str += "    *----*----*----*----*----*----*----*----*\n";
        for (int r = r8; r >= r1; --r) {
            char buf[256];
            const char *fmt = " %2d |%s|%s|%s|%s|%s|%s|%s|%s| %2d\n";
            snprintf(buf, sizeof(buf), fmt,
                     r + 1,
                     board[r][ca].toString(),
                     board[r][cb].toString(),
                     board[r][cc].toString(),
                     board[r][cd].toString(),
                     board[r][ce].toString(),
                     board[r][cf].toString(),
                     board[r][cg].toString(),
                     board[r][ch].toString(),
                     r + 1);
            str += buf;
            str += "    *----*----*----*----*----*----*----*----*\n";
        }
        str += "       a    b    c    d    e    f    g    h  \n";
    }

    void toString(const Moves& moves, std::string& str) const
    {
        str.clear();
        bool first = true;
        MovesCItr itr = moves.begin();
        for (; itr != moves.end(); ++itr) {
            if (!first) { str += ","; }
            first = false;
            (*itr).toStringMove(str); 
        }
    }

    void toStringMoves(std::string& str) const
    {
        toString(gameMoves, str);
    }

    void toString(const std::list<Move>& moves, std::string& str) const
    {
        str.clear();
        bool first = true;
        std::list<Move>::const_iterator itr = moves.begin();
        for (; itr != moves.end(); ++itr) {
            if (!first) { str += ","; }
            first = false;
            (*itr).toStringMove(str); 
        }
    }

    void whitePiecesStr(std::string& str) const
    {
        sidePiecesStr(whitePieces, str);
    }

    void whiteCapturedPiecesStr(std::string& str) const
    {
        sidePiecesStr(whiteCapturedPieces, str);
    }

    void blackPiecesStr(std::string& str) const
    {
        sidePiecesStr(blackPieces, str);
    }

    void blackCapturedPiecesStr(std::string& str) const
    {
        sidePiecesStr(blackCapturedPieces, str);
    }

    void insert(Moves& pieces, const Move& piece) const
    {
        pieces.insert(piece);
    }

    void update(Moves& pieces, const Move& from, const Move& to)
    {
        remove(pieces, from);
        if (from.getPiece() != to.getPiece()) {
            // piece was promoted
            setBoardPiece(to.rowT(), to.colT(), to.getPiece(), to.getSide());
            promotion = true;
        }
        insert(pieces, to);
    }

    void remove(Moves& pieces, const Move& piece) const
    {
        pieces.erase(piece);
    }

    void sidePiecesStr(const Moves& pieces, std::string& str) const
    {
        str.clear();
        bool first = true;
        MovesCItr itr = pieces.begin();
        for (; itr != pieces.end(); ++itr) {
            if (!first) { str += ","; }
            first = false;
            std::string tmp;
            (*itr).toStringPiece(tmp);
            str += tmp;
        }
    }

    void setAttacks(const Moves& attacks, Side side)
    {
        MovesCItr itr = attacks.begin();
        for (; itr != attacks.end(); ++itr) {
            const Move& move = *itr;
            Row r = move.rowT();
            Col c = move.colT();
            if (side == White) {
                // see if white is attacking the black king
                if ((board[r][c].getSide() == Black) && (board[r][c].getPiece() == King)) {
                    checkAttacker = &move;
                    blackCheck = true;
                }
                board[r][c].incrAttackWhite();
            } else {
                // see if black is attacking the white king
                if ((board[r][c].getSide() == White) && (board[r][c].getPiece() == King)) {
                    checkAttacker = &move;
                    whiteCheck = true;
                }
                board[r][c].incrAttackBlack();
            }
        }
    }

    void clearAttacks()
    {
        checkAttacker = NULL;
        whiteCheck = false;
        blackCheck = false;
        for (int r = r1; r <= r8; ++r) {
            for (int c = ca; c <= ch; ++c) {
                board[r][c].clearAttacks();
            }
        }
        promotion = false;
        castle = false;
        enpassant = false;
    }

    bool getCheck(Side player) const
    {
        return (player == White) ? whiteCheck : blackCheck;
    }

    bool wasPromotion() const { return promotion; }
    bool wasCastle() const { return castle; }
    bool wasEnpassant() const { return enpassant; }

private:
    const bool EN_PASSANT;
    unsigned seed;
    Turn turn;
    Square board[rMax][cMax];
    Castle white;
    Castle black;
    const Move *checkAttacker;
    bool whiteCheck;
    bool blackCheck;
    bool promotion;
    bool castle;
    bool enpassant;
    Pieces whitePieces;
    Pieces whiteCapturedPieces;
    Pieces blackPieces;
    Pieces blackCapturedPieces;
    Pieces movesCheck;
    std::list<Move> gameMoves;
};

#endif
