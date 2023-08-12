function out = PPlanaMan( images )
    %PPLANAAUT Proyeccion plana para 3 fotografias
    
    %Leer imagenes
    img1 = images(:,:,:,1);
    img2 = images(:,:,:,2);
    img3 = images(:,:,:,3);
    
    H = Homography_Manual(img1,img2);
    tmp = Unir(img1,img2,H);
    
    H = Homography_Manual(img3,tmp);
    out = Unir(img3,tmp,H);

end

