<form action="sell.php" method="post">
    <fieldset>
        <div class="form-group">
            <select class="form-control" name="symbol">
                <option disabled selected value="">Symbol</option>
                <?php foreach ($stocks as $stock): ?>
                    <option value="<?= $stock["symbol"] ?>"><?= $stock["symbol"] ?></option>
                <?php endforeach; ?>
            </select>
        </div>
        <div class="form-group">
            <button class="btn btn-danger" type="submit">Sell all shares</button>
        </div>
    </fieldset>
</form>