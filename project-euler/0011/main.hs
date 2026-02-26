numAt :: Int -> Int -> String -> Int
numAt x y contents = read $ (map words $ lines contents) !! y !! x

rows = [[(x,y), (x+1, y), (x+2,y), (x+3, y)] | x <- [0..16], y <- [0..19]]
cols = [[(x,y), (x, y+1), (x,y+2), (x, y+3)] | x <- [0..19], y <- [0..16]]
ldiag = [[(x,y), (x+1, y+1), (x+2, y+2), (x+3, y+3)] | x <- [0..16], y <-[0..16]]
rdiag = [[(x,y), (x+1, y-1), (x+2, y-2), (x+3, y-3)] | x <- [0..16], y <-[3..19]]

prodAt :: [(Int,Int)] -> String -> Int
prodAt lst contents = product $ map getNum lst
    where getNum (x,y) = numAt x y contents

result contents = foldl1 max $ map (flip prodAt contents) allTuples
    where allTuples = (rows ++ cols ++ ldiag ++ rdiag)

main = do
    contents <- readFile "input"
    putStrLn $ show $ result contents

