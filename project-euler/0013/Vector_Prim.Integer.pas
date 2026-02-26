unit Vector_Prim.Integer;

interface
uses
    Prim;

type
    TVector = record
        len: Longint;
        capacity: Longint;
        buf: Array of Integer;
    end;
    PElem = ^Integer;
    TElem = Integer;

procedure init(var this: TVector);
function get_elem(var this: TVector; n: Longint): PElem; 
procedure set_elem(var this: TVector; n: Longint; elem: Integer);
procedure push_elem(var this: TVector; elem: Integer);

implementation
procedure init(var this: TVector);
begin
    this.len := 0;
    this.capacity := 16;
    setLength(this.buf, 16);
end;

function get_elem(var this: TVector; n: Longint): PElem;
begin
    get_elem := @(this.buf[n]);
end;

procedure set_elem(var this: TVector; n: Longint; elem: Integer);
begin
    this.buf[n] := elem;
end;

procedure push_elem(var this: TVector; elem: Integer);
var
    newcap: Longint;
begin
    if this.len = this.capacity - 1 then
    begin
        newcap := this.capacity * 2;
        setLength(this.buf, newcap);
        this.capacity := newcap;
    end;
    this.buf[this.len] := elem;
    this.len := this.len + 1;
end;

end.


