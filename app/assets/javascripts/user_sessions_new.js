function selectUser() {
  document.getElementById("user_email").value =
    document.getElementById("user_name").value + "@hipbox.com"
}

document.addEventListener('DOMContentLoaded', (event) => {

  // Event listener to change user password to match the
  // selected name
  selectUser()
  document.getElementById("user_name")
    .addEventListener('change', selectUser.bind(null))
  // --

})
