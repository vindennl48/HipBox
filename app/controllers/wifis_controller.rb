class WifisController < ApplicationController
  before_action :authenticate_user!
  before_action :is_admin
  before_action :set_wifi, only: [:edit, :update, :destroy]

  # GET /wifis
  # GET /wifis.json
  def index
    @wifis = Wifi.all
  end

  # GET /wifis/1
  # GET /wifis/1.json
  def show
  end

  # GET /wifis/new
  def new
    @wifi = Wifi.find_or_create_by(name: "New Wifi", network: "", password: "")
    redirect_to wifis_path
  end

  # GET /wifis/1/edit
  def edit
  end

  # PATCH/PUT /wifis/1
  # PATCH/PUT /wifis/1.json
  def update
    respond_to do |format|
      if @wifi.update(wifi_params)
        format.html { redirect_to wifis_path, notice: 'Wifi was successfully updated.' }
        format.json { render :show, status: :ok, location: @wifi }
      else
        format.html { render :edit }
        format.json { render json: @wifi.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /wifis/1
  # DELETE /wifis/1.json
  def destroy
    @wifi.destroy
    respond_to do |format|
      format.html { redirect_to wifis_url, notice: 'Wifi was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_wifi
      @wifi = Wifi.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def wifi_params
      params.require(:wifi).permit(:name, :network, :password)
    end

    def is_admin
      if current_user != User.find_by(name: "Mitch")
        redirect_to channels_path
      end
    end
end
