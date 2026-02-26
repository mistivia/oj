triplets = foldl1 (++) $ map triplets1 [1..499]
    where triplets1 x = foldl1 (++) $ map triplets2 [x+1..999-x]
            where triplets2 y = [(x,y,1000-x-y)]

theTripet = filter (\(a,b,c) -> a^2 + b^2 == c^2) triplets

tripletProduct (a,b,c) = a * b * c

main = do
    putStrLn $ show $ tripletProduct $ head theTripet

