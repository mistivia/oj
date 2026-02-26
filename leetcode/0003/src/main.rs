struct Solution {}

impl Solution {
    pub fn length_of_longest_substring(s: String) -> i32 {
        let mut set: [u8;256] = [0;256];
        let mut i1: usize = 0;
        let mut i2: usize = 0;
        let mut max: i32 = 0;
        loop {
            if i1 >= s.len() { break; }
            if set[s.as_bytes()[i1] as usize] == 0 {
                set[s.as_bytes()[i1] as usize] = 1;
                i1 = i1 + 1;
                if i1 as i32 - i2 as i32 > max { max = i1 as i32 - i2 as i32 }
                continue;
            } else {
                loop {
                    set[s.as_bytes()[i2]  as usize] = 0;
                    i2 = i2 + 1;
                    if set[s.as_bytes()[i1] as usize] == 0 {
                        break;
                    } else {
                        continue;
                    }
                }
            }
        }
        max
    }
}

fn main() {
    println!("{:?}", Solution::length_of_longest_substring("abcabcbb".to_string()));
}