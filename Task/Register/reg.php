<html>
<head><title>Registering</title>
</head>
<body>
   <p>
<?php
/*$servername = "localhost";
$username = "root";
$password = "admin";
$dbnam ="phptest";*/
//mysql_select_db("phptest");
$db=mysql_connect("localhost", "root", "admin");
mysql_select_db("phptest");
//create connection
//$conn = new mysqli($servername, $username, $password);
//check connection
if(!$db){
    die("Connection Failed: ".$conn->connect_error);

}
$sql="INSERT INTO users (name, userid, pass, desg, email)
VALUES ('$_POST[name]','_POST[userid]','$_POST[pass]','$_POST[desg]','$_POST[email]')";

if($query($sql)==TRUE) {
    echo "Registered successfully!!!";

}   else  {
    echo "Error: ".sql."<br>".$conn->error;

}
//$conn->close();
?>
</p>
</body>
</html>
