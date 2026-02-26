impl Solution {
    fn char_to_int(c: char) -> i32 {
        match c {
            'I' => 1,
            'V' => 5,
            'X' => 10,
            'L' => 50,
            'C' => 100,
            'D' => 500,
            'M' => 1000,
            _ => 0,
        }
    }
    pub fn roman_to_int(s: String) -> i32 {
        let mut sum = 0;
        for i in 0..s.len() {
            if i < s.len() - 1 {
                if Self::char_to_int(s.as_bytes()[i as usize] as char)
                        < Self::char_to_int(s.as_bytes()[i+1 as usize] as char) {
                    sum = sum - Self::char_to_int(s.as_bytes()[i as usize] as char);
                    continue;
                }
            }
            sum = sum + Self::char_to_int(s.as_bytes()[i as usize] as char);
        }
        sum
    }
}

struct Solution {}
fn main() {
    println!("{}", Solution::roman_to_int("MCMXCIV".to_string()));
}