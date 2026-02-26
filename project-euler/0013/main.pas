program main;

uses
    Vector_Prim.Integer;

type
    TVec = Vector_Prim.Integer.TVector;

procedure revvec(var vec: TVec);
var
    i: Longint;
    j: Longint;
    t: Integer;
begin
    i := 0;
    j := vec.len-1;
    while i < j do
    begin
        t := vec.buf[i];
        vec.buf[i] := vec.buf[j];
        vec.buf[j] := t;
        i := i + 1;
        j := j - 1;
    end;
end;

function readnum(): TVec;
var
    v: TVec;
    c: Char;
begin
    init(v);
    while true do
    begin
        if eof then
            break;
        read(c);
        if c = #10 then
            break;
        push_elem(v, ord(c) - ord('0'));
    end;
    revvec(v);        
    readnum := v;
end;

function add(a: TVec; b: TVec): TVec;
var
    ret: TVec;
    carry, len, i, reg: Integer;
    na, nb: Integer;
begin
    init(ret);
    carry := 0;
    len := a.len;
    if b.len > len then
        len := b.len;

    for i := 0 to len-1 do
    begin
        na := 0;
        nb := 0;
        if i < a.len then
            na := a.buf[i];
        if i < b.len then
            nb := b.buf[i];
        reg := na + nb + carry;
        carry := reg div 10;
        push_elem(ret, reg mod 10);
    end;
    if carry <> 0 then
        push_elem(ret, 1);
    add := ret;
end;

procedure printnum(n: TVec);
var
    i: Integer;
begin
    for i := n.len-1 downto n.len-10 do
        write(chr(ord('0') + n.buf[i]));
    writeln('');
end;

const
    N = 100;
var
    nums: Array[1..N] of TVec;
    res: TVec;
    i: Integer;
begin
    init(res);
    for i := 1 to N do
        nums[i] := readnum();
    res := nums[1];
    for i := 2 to N do
        res := add(res, nums[i]);
    printnum(res);
end.
