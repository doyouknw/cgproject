<html>
<head><title>Registering</title>
</head>
<body>
   <p>
<?php
$servername = "localhost";
$username = "root";
$password = "admin"
//$dbnam ="phptest";
mysql_select_db("phptest");
//create connection
$conn = new mysqli($servername, $username, $password);
//check connection
if($conn->connect_error){
    die("Connection Failed: ".$conn->connect_error);

}
$sql="INSERT INTO users (name, userid, pass, desg, email)
VALUES ('$_POST[name]','_POST[userid]','$_POST[pass]','$_POST[desg]','$_POST[email]')";

if($conn->query($sql)==TRUE) {
    echo "Registered successfully!!!";

}   else  {
    echo "Error: ".sql."<br>".$conn->error;

}
$conn->close();
?>
</p>
</body>
</html>

