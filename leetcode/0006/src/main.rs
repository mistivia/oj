impl Solution {
    pub fn convert(s: String, num_rows: i32) -> String {
        if num_rows <= 1 {
            return s;
        }
        let mut x = 0;
        let mut y = 0;
        let mut path: Vec<(i32,i32)> = vec!();
        let mut is_down = true;
        for _ in 0..s.len() {
            path.push((x, y));
            if is_down {
                y = y + 1;
                if y == num_rows - 1 {
                    is_down = false;
                }
            } else {
                y = y - 1;
                x = x + 1;
                if y == 0 {
                    is_down = true;
                }
            }
        }
        let mut lines: Vec<String> = vec!();
        for _ in 0..num_rows {
            lines.push("".to_string());
        }
        for i in 0..s.len() {
            let (_,y) = path[i];
            lines[y as usize].push(s.as_bytes()[i] as char);
        }
        lines.join("")
    }
}

struct Solution {}
fn main() {
    println!("{}", Solution::convert("PAYPALISHIRING".to_string(), 3));
}