import System.Random

main :: IO ()
main = putStrLn ("Rolled " ++ (show =<< roll))

roll = randomR (1, 20 :: Int)

sayHello :: String -> IO ()
sayHello x =
  putStrLn ("Hello, " ++ x ++ "!")
