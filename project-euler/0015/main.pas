program main;

type
    Row = Array[0..20] of Int64;
    Grid = Array[0..20] of Row;

var
    gridpath: Grid;

procedure init_grid();
var
    i, j: Integer;
begin
    for i := 0 to 20 do
    begin
        for j := 0 to 20 do
            gridpath[i][j] := -1;
    end;
end;

procedure calc(row: Integer; col: Integer);
var
    r, c: Integer;
    ret: Int64;
begin
    ret := 0;
    if gridpath[row][col] <> -1 then
        exit;

    r := row + 1;
    c := col;
    if (r <= 20) and (c <= 20) then
    begin
        calc(r, c);
        ret := ret + gridpath[r][c];
    end;

    r := row;
    c := col + 1;
    if (r <= 20) and (c <= 20) then
    begin
        calc(r, c);
        ret := ret + gridpath[r][c];
    end;
    gridpath[row][col] := ret;
end;

begin
    init_grid();
    gridpath[20][20] := 1;
    calc(0, 0);
    writeLn(gridpath[0][0]);
end.

