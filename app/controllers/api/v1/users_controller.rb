class Api::V1::UsersController < Api::V1::BaseController

  def index
    puts "current_user: #{current_user}"
    render json: User.find(current_user.id)
  end

  def update
    user = User.find(params[:id])
    user.update_attributes(user_params)
    render json: user
  end

  def user_params
    params.require(:user).permit(:id, :email, :hpVol, :clickVol, :tbVol)
  end

end
