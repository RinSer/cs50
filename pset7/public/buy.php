<?php

    // configuration
    require("../includes/config.php");
    
    
    $user_id = $_SESSION["id"];
    
    if ($_SERVER["REQUEST_METHOD"] == 'GET')
    {
        // render buy form template
        render("buy_view.php", ["title" => "Buy"]);
    }
    elseif ($_SERVER["REQUEST_METHOD"] == 'POST')
    {
        // Get the user's cash
        $rows = CS50::query('SELECT cash FROM users WHERE id = ?', $user_id);
        $cash = $rows[0]["cash"];
        // Get the stock info
        $stock = lookup($_POST["symbol"]);
        $symbol = $stock["symbol"];
        $price = $stock["price"];
        // Check the share input
        if (preg_match("/^\d+$/", $_POST["shares"]) == true)
        {
            $shares = $_POST["shares"];
        }
        else
        {
            apologize("You can buy only whole shares!");
        }
        // Check if the operation is valid
        $cost = $price*$shares;
        if ($cost <= $cash)
        {
            // Add a new row to the db
            $portfolio = CS50::query('INSERT INTO portfolios (user_id, symbol, shares) VALUES(?, ?, ?) ON DUPLICATE KEY UPDATE shares = shares + VALUES(shares)', $user_id, $symbol, $shares);
            if ($portfolio == 0)
            {
                apologize("Could not buy the shares!");
            }
            else
            {
                $user = CS50::query('UPDATE users SET cash = cash - ? WHERE id = ?', $cost, $user_id);
                if ($user == 0)
                {
                    apologize("Could not receive your money!");
                }
                else
                {
                    // Add transaction to history
                    CS50::query('INSERT IGNORE INTO history (transaction, user_id, symbol, shares, price) VALUES ("BUY", ?, ?, ?, ?)', $user_id, $symbol, $shares, $price);
                    // Redirect to the main page on success
                    redirect("/");
                }
            }
        }
        else
        {
            apologize("You do not have enough cash!");
        }
    }

?>