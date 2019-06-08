function selectUser() {
  document.getElementById("user_email").value =
    document.getElementById("user_name").value + "@hipbox.com"
}

function selectUserListener() {
  selectUser()
  document.getElementById("user_name")
    .addEventListener('change', selectUser.bind(null))
}
