use std::collections::HashMap;

fn re_match(rs: &[u8], rp: &[u8], i: usize, j: usize, ht: &mut HashMap<(usize, usize), bool>) -> bool {
    if let Some(&val) = ht.get(&(i, j)) {
        return val;
    }

    if i == rs.len() && j == rp.len() {
        ht.insert((i, j), true);
        return true;
    }

    if j == rp.len() {
        ht.insert((i, j), false);
        return false;
    }

    if rp.len() - j >= 2 && rp[j + 1] == b'*' {
        let ret = re_match_star(rs, rp, i, j + 2, rp[j] as char, ht);
        ht.insert((i, j), ret);
        return ret;
    }

    if i == rs.len() {
        ht.insert((i, j), false);
        return false;
    }

    if rp[j] == b'.' {
        let ret = re_match(rs, rp, i + 1, j + 1, ht);
        ht.insert((i, j), ret);
        return ret;
    }

    if rp[j] != rs[i] {
        ht.insert((i, j), false);
        return false;
    }

    let ret = re_match(rs, rp, i + 1, j + 1, ht);
    ht.insert((i, j), ret);
    return ret;
}

fn re_match_star(rs: &[u8], rp: &[u8], i: usize, j: usize, c: char, ht: &mut HashMap<(usize, usize), bool>) -> bool {
    if i < rs.len() && (rs[i] as char == c || c == '.') {
        return re_match(rs, rp, i, j, ht) || re_match_star(rs, rp, i + 1, j, c, ht);
    }
    re_match(rs, rp, i, j, ht)
}

fn is_match(s: String, p: String) -> bool {
    let mut ht = HashMap::new();
    re_match((&s).as_bytes(), (&p).as_bytes(), 0, 0, &mut ht)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_match1() {
        assert_eq!(is_match("bbbba".to_string(), ".*a*a".to_string()), true);
    }

    #[test]
    fn test_basic_mismatch() {
        assert_eq!(is_match("aa".to_string(), "a".to_string()), false);
    }

    #[test]
    fn test_basic_match() {
        assert_eq!(is_match("aa".to_string(), "aa".to_string()), true);
    }

    #[test]
    fn test_empty_match1() {
        assert_eq!(is_match("a".to_string(), "ab*".to_string()), true);
    }

    #[test]
    fn test_dot_match() {
        assert_eq!(is_match("abc".to_string(), "a.c".to_string()), true);
    }

    #[test]
    fn test_star_match_one_or_more() {
        assert_eq!(is_match("aa".to_string(), "a*".to_string()), true);
    }

    #[test]
    fn test_star_match_zero() {
        assert_eq!(is_match("aab".to_string(), "c*a*b".to_string()), true);
    }

    #[test]
    fn test_dot_star_match() {
        assert_eq!(is_match("ab".to_string(), ".*".to_string()), true);
    }

    #[test]
    fn test_complex_cases() {
        assert_eq!(is_match("mississippi".to_string(), "mis*is*p*.".to_string()), false);
        assert_eq!(is_match("mississippi".to_string(), "mis*is*ip*.".to_string()), true);
    }
    
    #[test]
    fn test_empty_match() {
        assert_eq!(is_match("".to_string(), "".to_string()), true);
    }
}

fn main() {}