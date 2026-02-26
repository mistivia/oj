impl Solution {
    pub fn str_str(haystack: String, needle: String) -> i32 {
        if needle.len() > haystack.len() { return -1; }
        for i in 0..haystack.len() - needle.len()+1 {
            let mut failed = false;
            for j in 0..needle.len() {
                if haystack.as_bytes()[i+j] != needle.as_bytes()[j] {
                    failed = true;
                    break;
                }
            }
            if !failed { return i as i32; }
        }
        -1
    }
}
struct Solution { }
fn main() {
    println!("{}", Solution::str_str("abaaa".to_string(), "abb".to_string()));
}