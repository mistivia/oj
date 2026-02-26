with ada.text_io; use ada.text_io;
with ada.integer_text_io; use ada.integer_text_io;
with ada.containers.vectors;

procedure main is
    package int_vector is new ada.containers.vectors (
        index_type => natural,
        element_type => integer
    );
    use int_vector;
    
    function add(a: vector; b: vector) return vector is
        ret: vector;
        len: integer;
        na, nb: integer;
        carry: integer := 0;
    begin
        len := integer(length(a));
        if integer(length(b)) > len then
            len := integer(length(b));
        end if;
        for i in 0..len-1 loop
            na := 0;
            nb := 0;
            if i < integer(length(a)) then
                na := element(a, i);
            end if;
            if i < integer(length(b)) then
                nb := element(b, i);
            end if;
            append(ret, (na + nb + carry) mod 10);
            carry := (na + nb + carry) / 10;
        end loop;
        if carry > 0 then
            ret.append(1);
        end if;
        return ret;
    end add;

    function mul(a: vector; b: integer) return vector is
        ret: vector;
        len: integer;
        carry: integer := 0;
    begin
        len := integer(length(a));
        if b = 10 then
            append(ret, 0);
            for i in 0..len-1 loop
                append(ret, a.element(i));
            end loop;
            return ret;
        end if;
        
        if b = 0 then
            append(ret, 0);
            return ret;
        end if;
        
        for i in 0..len-1 loop
            append(ret, (a.element(i) * b + carry) mod 10);
            carry := (a.element(i) * b + carry) / 10;
        end loop;
        if carry > 0 then
            ret.append(carry);
        end if;
        return ret;
    end mul;

    procedure printnum(a: vector);

    function mul(a: vector; b: vector) return vector is
        ret: vector;
        t1: vector;
        t2: vector;
        len: integer;
    begin
        len := integer(length(b));
        for i in 0..len-1 loop
            clear(t1);
            clear(t2);
            for j in 1..i loop
                append(t1, 0);
            end loop;
            t2 := mul(a, b.element(i));
            for j in 0..integer(length(t2))-1 loop
                append(t1, t2.element(j));
            end loop;
            ret := add(ret, t1);
        end loop;
        return ret;
    end mul;

    procedure printnum(a: vector) is
    begin
        for i in reverse 0..integer(length(a))-1 loop
            put(a.element(i), 0);
        end loop;
        new_line;
    end printnum;

    function pow2(n: integer) return vector is
        t: vector;
    begin
        if n = 0 then
            append(t, 1);
            return t;
        end if;
        if n mod 2 = 1 then
            return mul(pow2(n-1), 2);
        else
            t := pow2(n / 2);
            return mul(t, t);
        end if;
    end pow2;

v: vector;
res: integer := 0;
begin
    v := pow2(1000);
    for i in 0..integer(length(v)) - 1 loop
        res := res + v.element(i);
    end loop;
    put(res);
end main;

