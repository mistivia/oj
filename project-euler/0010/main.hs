primes = primesImpl 2 []
    where
        primesImpl :: Integer -> [Integer] -> [Integer]
        primesImpl i primeList =
            if isPrime then
                i:primesImpl (i+1) (primeList ++ [i])
            else
                primesImpl (i+1) primeList
            where
                isPrime = isPrimeImpl primeList
                    where
                        isPrimeImpl [] = True
                        isPrimeImpl (x:xs) =
                            i `mod` x /= 0 && isPrimeImpl xs

main = do
    print (sum $ takeWhile (<2000000) primes)
