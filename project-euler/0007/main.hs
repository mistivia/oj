nthPrime :: Integer -> Integer
nthPrime targetCount = nextPrimeImpl 2 1 []
    where
        nextPrimeImpl i primeCount primeList = 
            if isPrime then
                if primeCount == targetCount then
                    i
                else
                    nextPrimeImpl (i + 1) (primeCount + 1) (primeList ++ [i])
            else
                nextPrimeImpl (i+1) primeCount primeList
            where
                isPrime =
                    isPrimeImpl primeList
                    where
                        isPrimeImpl [] = True
                        isPrimeImpl (x:xs) = i `mod` x /= 0 && isPrimeImpl xs

main = do
    putStrLn $ show $ nthPrime 10001
