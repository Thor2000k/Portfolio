npHelper :: Bool -> [Int] -> Int -> Int -> [Int]
npHelper p s n xs
  | p == False = (takeStones xs (fst(splitAt n s))) ++ [(s!!n) + (s !! ((2*n) - xs)) + 1] ++ (snd(splitAt (n + 1) (takeStones (2*n-xs) s)))
  | otherwise =  (takeStones (2*n - (xs)) (fst(splitAt (xs ) s))) ++ init (snd(splitAt (xs) (takeStones (xs) s))) ++ [s!!(2*n + 1) + 1 + (s!!((2 * n) - (xs))) ]  

takeStones :: Int -> [Int] -> [Int]   
takeStones xs s = (fst( splitAt xs s)) ++ [0] ++ (snd( splitAt (xs + 1) s )) 


--snd(splitAt xs s(snd(splitAt (xs + 1) s)))

--fst(splitAt (xs) s) ++ [s!!(xs) + snd( splitAt (n + 1) s)!!(xs ) + 1] ++ drop(xs + 1) (fst(splitAt (n ) s)) ++ takeStones xs (snd(splitAt (n + 1) s))


--