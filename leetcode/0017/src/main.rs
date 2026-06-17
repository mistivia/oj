const KEYMAP: [(i32, &str); 8] = [
    (2, "abc"),
    (3, "def"),
    (4, "ghi"),
    (5, "jkl"),
    (6, "mno"),
    (7, "pqrs"),
    (8, "tuv"),
    (9, "wxyz"),
];

fn add_char(result: Vec<String>, digit: i32) -> Vec<String> {
    // find in keymap
    let mut new_result: Vec<String> = Vec::new();
    let mut chars: &str = "";
    for pair in &KEYMAP {
        if pair.0 == digit {
            chars = pair.1;
            break;
        }
    }
    if chars == "" {
        return result;
    }
    for &byte in chars.as_bytes() {
        for s in &result {
            let mut newstr = s.clone();
            newstr.push(byte as char);
            new_result.push(newstr);
        }
    }
    new_result
}

impl Solution {
    pub fn letter_combinations(digits: String) -> Vec<String> {
        if digits.len() == 0 {
            return vec!();
        }
        let mut result = vec!(String::new());
        for &digit in digits.as_bytes() {
            result = add_char(result, digit as i32 - '0' as i32);
        }
        result
    }
}

struct Solution {}

fn main() {}
