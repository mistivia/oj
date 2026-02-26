impl Solution {
    fn check_odd(s: &String, i: i32) -> (i32, i32, i32) {
        let mut r = 0;
        loop {
            let next = r + 1;
            if i - next < 0
                    || i + next >= s.len() as i32
                    || s.as_bytes()[(i-next) as usize] != s.as_bytes()[(i+next) as usize] {
                break;
            }
            r = next;
        }
        let len = 2 * r + 1;
        let start = i - r;
        let end = i + r;
        (len, start, end)
    }

    fn check_even(s: &String, i: i32) -> (i32, i32, i32) {
        let mut r = 0;
        loop {
            let next = r + 1;
            if i - next + 1 < 0
                || i + next >= s.len() as i32
                || s.as_bytes()[(i-next+1) as usize] != s.as_bytes()[(i+next) as usize] {
                break;
            }
            r = next;
        }
        let len = 2 * r;
        let start = i - r + 1;
        let end = i + r;
        (len, start, end)
    }

    fn check_palindrome_len(s: &String, i: i32) -> (i32, i32, i32) {
        let (len1, start1, end1) = Self::check_odd(s, i);
        let (len2, start2, end2) = Self::check_even(s, i);
        if len2 > len1 {
            (len2, start2, end2)
        } else {
            (len1, start1, end1)
        }
    }

    pub fn longest_palindrome(s: String) -> String {
        let mut maxpalinlen = -1;
        let mut start: i32 = 0;
        let mut end: i32 = 0;
        for i in 0..s.len() {
            let (palin_len, pstart, pend) = Self::check_palindrome_len(&s, i as i32);
            if palin_len > maxpalinlen {
                maxpalinlen = palin_len;
                start = pstart;
                end = pend;
            }
        }
        s[start as usize ..(end + 1) as usize].to_string()
    }
}

struct Solution {}

fn main () {
    println!("{}", Solution::longest_palindrome("babad".to_string()));
    println!("{}", Solution::longest_palindrome("cbbd".to_string()));
}
