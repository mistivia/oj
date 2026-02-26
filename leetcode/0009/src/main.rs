impl Solution {
    pub fn is_palindrome(x: i32) -> bool {
        if x < 0 {
            return false;
        }
        let s = x.to_string();
        let rs: String = s.chars().rev().collect();
        return s == rs;
    }
}

struct Solution {}
fn main() {}