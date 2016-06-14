<?php

    // configuration
    require("../includes/config.php");
    
    if (empty($_POST["symbol"]))
    {
        // render quote page with form
        render("quote_view.php", ["title" => "quote"]);
    }
    else
    {
        // Get stock price
        $stock = lookup($_POST["symbol"]);
        // Check if stock has been received
        if (empty($stock))
        {
            apologize("The stock has not been found!");
        }
        else
        {
            // render quote page with form
            render("quote_view.php", ["title" => "quote", "stock" => $stock]);
        }
    }

?>