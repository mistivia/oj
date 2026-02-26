impl Solution {
    pub fn longest_common_prefix(strs: Vec<String>) -> String {
        if strs.is_empty() { return "".to_string(); }
        let mut l: i32 = -1;
        let mut brk = false;
        for i in 0..strs[0].len() {
            let c = strs[0].as_bytes()[i];
            for j in 1..strs.len() {
                if strs[j].len() <= i || strs[j].as_bytes()[i] != c {
                    brk = true;
                    break;
                }
            }
            if brk { break; }
            l = l + 1;
        }
        if l < 0 {
            return "".to_string();
        }
        strs[0][0..(l+1) as usize].to_string()
    }
}

struct Solution {}
fn main() {
    println!("{}", Solution::longest_common_prefix(vec![
        "fliower".to_string(),
        "fliow".to_string(),
        "fliight".to_string()]));
}