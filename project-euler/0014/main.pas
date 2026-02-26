program main;

const N = 1000000;

var
    res: array[1..N] of longint;

function getlen(x: int64): longint;
var
    r: longint;
    sp: longint;
    argstk: array[1..1000] of int64;
begin
    sp := 0;
    while (x > N) or (res[x] = -1) do
    begin
        sp := sp + 1;
        argstk[sp] := x;
        if x mod 2 = 1 then
            x := x * 3 + 1
        else
            x := x div 2;
    end;
    r := res[x];
    while sp > 0 do
    begin
        if argstk[sp] <= N then
            res[argstk[sp]] := r + 1;
        r := r + 1;
        sp := sp  - 1;
    end;
    getlen := r;
end;

var
    len: longint;
    i: longint;
    maxres: longint;
    maxlen: longint;

begin
    len := 0;
    maxres := 0;
    maxlen := 1;
    i := 1;
    while i <= N do
    begin
        res[i] := -1;
        i := i + 1;
    end;
    res[1] := 1;

    i := 1;
    while i <= N do
    begin
        len := getlen(i);
        if len > maxlen then
        begin
            maxres := i;
            maxlen := len;
        end;
        i := i + 1;
    end;
    writeln(maxres);
end.
