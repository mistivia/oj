oneDigitLen n = [3,3,5,4,4,3,5,5,4] !! (n-1)

teen n = [3,6,6,8,8,7,7,9,8,8] !! (n-10)

dozens n = tensPart + onesPart
    where
        tens = (n `div` 10) * 10
        ones = n `mod` 10
        tensPart = case tens of
            20 -> 6
            30 -> 6
            40 -> 5
            50 -> 5
            60 -> 5
            70 -> 7
            80 -> 6
            90 -> 6
        onesPart = if ones == 0 then 0 else oneDigitLen ones

twoDigitLen n
    | n < 0    = 0
    | n < 10    = oneDigitLen n
    | n < 20    = teen n
    | otherwise = dozens n

hundreds n
    | n `mod` 100 == 0 = oneDigitLen (n `div` 100) + 7
    | otherwise        = oneDigitLen (n `div` 100) + 10 + twoDigitLen (n `mod` 100)

numberLen n
    | n == 1000 = 11
    | n >= 100  = hundreds n
    | n >= 20   = dozens n
    | n >= 10   = teen n
    | otherwise = oneDigitLen n

main = do
    print $ sum $ map numberLen [1..1000]
