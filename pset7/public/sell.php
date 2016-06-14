<?php

    // configuration
    require("../includes/config.php");
    
    
    $user_id = $_SESSION["id"];
    
    if ($_SERVER["REQUEST_METHOD"] == 'GET')
    {
        $rows = CS50::query('SELECT symbol FROM portfolios WHERE user_id = ?', $user_id);
        // Store the stock symbols in array
        $stocks = [];
        foreach ($rows as $row)
        {
            $stocks[] = ["symbol" => $row["symbol"]];
        }
        // Apologize if there's nothing to sell
        if (empty($stocks))
        {
            apologize("Nothing to sell!");
        }
        else
        {
            // Render the view
            render("sell_view.php", ["title" => "Sell", "stocks" => $stocks]);
        }
    }
    elseif ($_SERVER["REQUEST_METHOD"] == 'POST')
    {
        // Get the symbol of sold stock
        $symbol = $_POST["symbol"];
        // Find the db entry for this stock
        $rows = CS50::query('SELECT shares FROM portfolios WHERE user_id = ? AND symbol = ?', $user_id, $symbol);
        $shares = $rows[0]["shares"];
        // Find the current stock price
        $current_stock = lookup($symbol);
        $money = $shares*$current_stock["price"];
        // Delete the necessary db entry
        $deleted = CS50::query('DELETE FROM portfolios WHERE user_id = ? AND symbol = ?', $user_id, $symbol);
        if ($deleted == 0)
        {
            apologize("Could not proceed the operation!");
        }
        else
        {
            $updated = CS50::query('UPDATE users SET cash = cash + ? WHERE id = ?', $money, $user_id);
            if ($updated == 0)
            {
                apologize("Could not receive the money!");
            }
            else
            {
                // Add transaction to history
                CS50::query('INSERT IGNORE INTO history (transaction, user_id, symbol, shares, price) VALUES ("SELL", ?, ?, ?, ?)', $user_id, $symbol, $shares, $current_stock["price"]);
                // Redirect to the main page
                redirect("/");
            }
        }
    }
    
    
    
?>