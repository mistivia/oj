import Data.Char (digitToInt)
import Data.Maybe (catMaybes)

findMax13 numbers =
    foldl1 max $ map (\i -> subArrayProduct i 13) [0..((length numbers) - 13)]
    where
        subArrayProduct start length =
            product $ take length (drop start numbers)

main = do
    contents <- readFile "input"
    let max13 = findMax13 $ map digitToInt $ filter isDigit contents in
        putStrLn $ show max13
    where isDigit x = x <= '9' && x >= '0'
