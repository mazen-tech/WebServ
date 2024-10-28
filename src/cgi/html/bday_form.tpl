<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" type="text/css" href="style.css">
    <title>Birthday Form</title>
</head>
<body>
    <h1>Find best date for shared BirthDay</h1>
    <form method="get" action="bday_share.tpl">
        <div style="display: flex; gap: 20px;">
            <div style="width: 40%;">
                <label for="yn">your name</label>
                <input type="text" name="your_name" id="yn">
                <label for="yb">Insert your 2024's birthday</label>
                <input type="date" name="your_bday" id="yb" required>
            </div>
            <div style="width: 40%;">
                <label for="fn">your friend's name </label>
                <input type="text" name="friend_name" id="fn">
                <label for="fb">Insert your friend's 2024's birthday</label>
                <input type="date" name="friend_bday" id="fb" required>
            </div>
        </div>
        <input type="submit" value="Calculate" class="button">
    </form>
    <p><a href="index.html">take me home</a></p>
</body>
</html>
