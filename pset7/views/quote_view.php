<div>
    <?php if (isset($stock)): ?>
        <p>A share of <?= htmlspecialchars($stock["name"]) ?> (<?= htmlspecialchars($stock["symbol"]) ?>) costs <b>&#36;<?= number_format($stock["price"], 2) ?></b></p>
    <?php else: ?>
        <form action="quote.php" method="post">
            <div class="form-group">
                <input autocomplete="off" autofocus class="form-control" name="symbol" placeholder="Symbol" type="text"/>
            </div>
            <div class="form-group">
                <button class="btn btn-info" type="submit">
                    <span aria-hidden="true" class="glyphicon glyphicon-log-in"></span>
                    Get Quote
                </button>
            </div>
        </form>
    <?php endif; ?>
</div>