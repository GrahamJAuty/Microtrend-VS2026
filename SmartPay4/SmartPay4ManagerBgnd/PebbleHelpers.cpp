//$$******************************************************************
#include "CkBinData.h"
#include "CkJsonObject.h"
#include "CkJsonArray.h"
#include "CkJwt.h"
#include "CkStringBuilder.h"
//$$******************************************************************
#include "..\SmartPay4Shared\Prompter.h"
#include "..\SmartPay4Shared\ServerData.h"
//$$******************************************************************
#include "PebbleHelpers.h"
//$$******************************************************************

CPebbleHelpers::CPebbleHelpers()
{	
	//DEMO KEY
	{
		CString strBase64 = "";
		strBase64 += "MIIJQgIBADANBgkqhkiG9w0BAQEFAASCCSwwggkoAgEAAoICAQDCgSBj+rrohCpb";
		strBase64 += "d0FniAiMw/W/gWN3a2MM5MTYRuDTlLCA+vE0creNa+rqA/zRCKZDxJIFJq3AJFk2";
		strBase64 += "9OnNxupfzZUa9nH/cP3tQGNzqWYIPhBUvJOV71L1IL7nQuGYqYcxg7pZOQz3r+Ym";
		strBase64 += "nhjXFSQzvbgZ3CXnye1TkVV1PsPoPBZ71yuIIiVvRpnarrBtfRmICVZPL1Mj/NeF";
		strBase64 += "0nDVgalB4lOFY4qiW44jSJfOG09mXFKLJddvXwo56XJjqjGemd3MTJE3GQQYcCZa";
		strBase64 += "IPVlWiMUctTIeshkyYJ5AGaAWMNTSWkp2EkbQygIKmd9hhS+Tft/vZrzCZzX66dL";
		strBase64 += "jSv45Yfb0HsFUfpMMuXdooBQ7lL2bAXDX/zbjyIlYhif/Ugojm3idcgX9oqjdjG8";
		strBase64 += "Sdlm0h6lFvjLomP0el6rUvznoQQ72STuQZfvQp6tBvrE9OR1FxlnKvvcOs8J+JqC";
		strBase64 += "+/bPxIwq+lWMe+EtpWQDAXAvEOkszfeywWIyuQIYIeCkPtdGPW4hjV8qW4lG+rOO";
		strBase64 += "MLu6dDYEwQvVQ76k3XkF+12ui1pKJpte8pmK+E+Fu//B/AgpUW71vPL2tyvqFX+4";
		strBase64 += "EaDSoqiU7YpwaTfCGPBzCw4ptiUN9bRpNLhnp5A7jJyrmeVjLc0FPcMnwIDRkGLD";
		strBase64 += "h769v49yRNDcwHXKV8gMLzULOOYcJwIDAQABAoICAB4ElFLPG6Yjh5n4IgrwDp8S";
		strBase64 += "5cwOvvgWMFeJaEEXUelgIDa3rnQcDuyaLstmSXqamCELUZx5PLApAUUfBS+x4Z1e";
		strBase64 += "0AKKQvZboi90aXosjEenXxkvb/jgJ5sXLlSTQ4KDtLlVOB7pqXEBykZEI1dGYauZ";
		strBase64 += "PbQYABhGrK7iAMtdysRTnwYGeNHDW1L8wA/k4pZIi/aCYmSezCOuCOET+j/+iSMB";
		strBase64 += "jUb1IYHZxBT5RhxpUJaz7DVMu3PqZLH4BPm8Bs5ltrpUnXC1wVENizhhBR5a4TZt";
		strBase64 += "sR3Y/TEOJxRflcd8a3YLdVnKPHYi+3xPIbZYnr5EBhV6gKho37Y0c0GQtwGXAva1";
		strBase64 += "HX/PGFDafoCHNl04PMyuNrKZfP8vaYuI77nP0MHSJyIpEPb75EnhvEbFXF/9tUZA";
		strBase64 += "MDwzfhnC8PTMllr1onLjBqeS4MXL3UIxcYj4FoTBcsVw27w/m9CfPTK2jEtdbdHU";
		strBase64 += "mJFypGoAb3RsML6T6p/nXQO6TDi7oKJeC2X0UdvyNatILCb3X0S5bdq1XDcVma/C";
		strBase64 += "rnZYOWUp+e/PpSig+zEf2hCgvEBsW17oCJnZt8oQsJz9wPPwh0qkwOLpg+HbzBDQ";
		strBase64 += "sVUVyiNsLFLr5ieO64ilj3jOubSacDDv9G49OO/hFWthLhCo4M5zRaHlRGV9wqCC";
		strBase64 += "mqhTuGfsxrV4pmJyJ51JAoIBAQDnvE+4k4GLGjim0G78xW9M6+FNReG1RpgO7caN";
		strBase64 += "wcvSSHz2xNjvW6kS6525t4wHrYpZ9Pg524pWqK3rpooACYaD8vZQCMO87PI3PV4n";
		strBase64 += "1n5cXSLYRJjl2ofm7v3/knfJ52KAxXaViiAP//z+Nuro2UBXu0eY0J8PWG2Vox1l";
		strBase64 += "Ttdzj388ncj/TlzvgEDUFg1z25D0nxgwQxkh8yqNDjkFRfQJqo9FjG4XpRx1VZyV";
		strBase64 += "cvydlnR+OK2pVGYq8W/Lc1WPYOmiNVI+5uCrKZTKJhaSUdsZRdYjR/ToKH9UtWdR";
		strBase64 += "jjbeYXXQSxsA2OasgcxbM6Ojy9JdqWRk2v9la+pDfdjbgBn7AoIBAQDW3tSZHrVS";
		strBase64 += "yx7tsWg8ImGx+BKl3S+38JYlZjE3TdPZVrxdlSb2F4mpgcFYQnRKBHjqdeQR0crG";
		strBase64 += "PMz++lBmasdq+VM3kW5PLH5CWEIirSvoaoqt2t0pO8+I9qap9IG3ACgDDGbYojWU";
		strBase64 += "AQMPa0+qHrFYUih5VgujjbkxpIhVVPYL9yXIZWgN55sqqemmRxZS0jRgsE8PDSgK";
		strBase64 += "nbJamkUzxhtmVi9a69vscLi/479fvbgij/9+V9W34ZN9U7r/OpK5WEY1tH5tSmVM";
		strBase64 += "NFnfldZiVwKs+t7PMmMANsq3uJsdfl5DsZu680qPAWyn21V5t2smKfCuHd7oKUib";
		strBase64 += "++mC3aZKafrFAoIBAQCW2cjzmGIN0SXrwWdeeI+5fRuPF9yUCr5ViYdlJDCwVKu0";
		strBase64 += "X/Pv7LUlMefcy5UGMnwLjyI/V6C4RNniuui+mXTNVE3vT6VLyNrsBrDSMhjTu3VB";
		strBase64 += "X3ZAun8ZX55MzmXYMx22QdSuB9pTUAq62bkVlscB1VIIMlY4wRGlBSfPmQE7or/r";
		strBase64 += "D7OWLsbkyZJp/WkIcHfuJdqzIBquNQMQC6AmxKDV4ged+Ip6LetWnkJuLwou85Al";
		strBase64 += "0nOJ9P0FG2HxCMU7rpSoGRnPBLBf+5N1/FFZMpvz2bhKAz90GVi6JdR6DJ+/J6Zv";
		strBase64 += "z1Rmust+pIgJHpZ5u+bXPbuyw7TZquQ2xydxy/QdAoIBAFrWVVZuTwWzbV3bVE+n";
		strBase64 += "0VlloKySN6wxf6hL7Z6NvnPxYSzG83OyO7Z3bn+biIIKJfwo3sRRxsHLznY06GyD";
		strBase64 += "0857nRyu1M2P58iwezUhtKBK+lnJXxohePlUHN1YysXMwR2Yzpni6Xeqfdq06Tbo";
		strBase64 += "Rt0pzoaNQ9pe/d3oyuIYSvSABW8Uv8qxET4WGvr6rotQ5mfB4Ypha34hcNcwxPTA";
		strBase64 += "WbEH2nk7oPvyI4isBam6EPA7pC2LgIR+TtX64eEoqALHONdXeONHQk/XCe+l7r4g";
		strBase64 += "PrdWT3+Us5nRkB3fY2QymjNQEKAWXnzag07rQNV2H0xBCsjxQ90uJs4az3tNrRFk";
		strBase64 += "gGkCggEAP8uxG8H62y7YY9RdEzJypWB8OH+0+ObUzxK4hlHj4oYSgKmzhjHlIiGe";
		strBase64 += "D5SfeppBtIXSv4c3N/Y0a0p47n/hEW/GH8qppx4A1JetSLUAYmpRjMSC98Cpvpbv";
		strBase64 += "5NVmsmGoFk1Kvagnqg2q5gKPdor2EZSg1LJcYh4hjhxYPZVCzoEboy3FGRFjALsf";
		strBase64 += "eCwiePdKDhE/dWIkvftJtnN3zy4ZscwkdnZ/u6oZVehKZno8wV/8JE8HSUV32OfU";
		strBase64 += "wTfaRyb+/M/rAvBKngQHPUtOHU1N+Ole8WuELYyocmSZkIWof5KkGnJ6xrvKiDb7";
		strBase64 += "oqI1Rscki9eO2uyMkhZZA1c7TkyV4Q==";

		CkBinData BinData;
		BinData.LoadEncoded(strBase64, "base64");
		m_PrivateKeyDemo.LoadAnyFormat(BinData, "");
	}

	//PRODUCTION KEY
	{
		CString strBase64 = "";
		strBase64 += "MIIJQwIBADANBgkqhkiG9w0BAQEFAASCCS0wggkpAgEAAoICAQC0PD9RRcOHeU46";
		strBase64 += "KK4VbHXxUfnV1wDb8wRZTw/d6EzoAbl9IRyVOuBHjb9SM0TLgNZCOdjlHThV10xM";
		strBase64 += "2nhFEshCI8YuMQe5u01v/VeGte9kMWrI2vZk4kF7vm71uqlyBe6EXt5MqR0DkhBz";
		strBase64 += "aOJVY0SC2P1YdvZVHiYfTfGp5kkTg5Aw1ZiA4Gx2c0NP6bqx2au4ZcjfNBJYIofC";
		strBase64 += "fvRbuj7x2lUDy0OSinydkQdvDSzDlACg2bMfDmIrsdTQjgbMGIhf4KMEh798g0IC";
		strBase64 += "8+Gsx6xf/xVUogkOzBUdYx224AF+9AY+PRt/tqWG5NHR6BKpe9wfp5HEeE21ZVX4";
		strBase64 += "Y/kHb/U8wKK8Jna2hp92hbAdMjh/chB0KmZPnVNzPoAAWqoxbeuGxmer5NxRVs3d";
		strBase64 += "z+DPMLmshaFHsj/8MtDFuvWXViJjvmlbeby2L3sFOUSS3Uqb22f+dDiSIyf6e2td";
		strBase64 += "UNeYtCEE7Fle5fPu86uEkjeRB6asZwWhaqP2wI6HZ2ZFiIa2ui3igbNIY7v6jWP1";
		strBase64 += "rMgCSN7Mmk1qsYK+0abhXA2F4GUKIHYXhpYX9ooron6e5/6blpveJA6/qmUWOoxw";
		strBase64 += "Kr+UCtQR47ZeHdwo1DaLfeeOI6oTtEATg7V/fesVqy7IuJKC1Sv1Uyb9cktJDgW1";
		strBase64 += "XmrKc/9x1mBWujY5mO6Z3ZkAlvI6gwIDAQABAoICAD8/AwMMhZwlC7fp7Yi0CkE4";
		strBase64 += "AIEzFOiaAxUp4sn+ifpCHKqVs3eEdQ525cO9HO+Cku0jaoKfudSZm/s+EMVAXYik";
		strBase64 += "EBfGec0JtpdNYejyMALLOosoFx14rfzz6vEM4wgRn8zQk5Z4I6fMmjw9emr3TiPS";
		strBase64 += "bDieajudPEhw7BQwLMWNvcv9MCpNVpJMSTsiLDLbzOAEy7PTjHpkB5rPPQVxOkRZ";
		strBase64 += "VsrhiaFDOo56RY8JIwOwjHXlXu9Pd1CY8zKKHEWDeFuYsuvUjPPvBXuOxEsCuHDp";
		strBase64 += "TL1prN8F64Tn8pXpTm0TyZsOi3dKCxTmd+B+XQofXjO3dDXq923nlP74aftbpMTo";
		strBase64 += "kBgSWYwdG7Y6XW8bAVvCuz3f8lQfkvN+CYnJdV8dVM0y5WV5/ftrAPyjGc7ISccG";
		strBase64 += "HF2aSv9EvKQQOI8tiki8icEmzYGNq2MHRvjMm6j9p0fPN8BDdVxoCysSng/isvCv";
		strBase64 += "5iLtILxjh8uVdUD88nsSf+kPsP29xtqCmVO/fcmWumaJW4NqOJKpJBXm5Xq6G0rZ";
		strBase64 += "jcoETGUUnSl57Sep0MMGP81mJdZ8I9Dc+HmpALgjcZAFY031xna1nZ6teu0ieN5P";
		strBase64 += "oPeYR27UFBn+sWujdsU7nmIQ++OnEnvBJOHoQL3DuEoW74OUnVOSi/bohlE1UoZN";
		strBase64 += "6Tm1b3gRqXuyZLV+8YcZAoIBAQDxtUJspm77D6tG1yb0mra/sHDsVS8Yxf8Z5aLR";
		strBase64 += "fnncbq/7U98ZNxJT/aBBS/7KhCe9reKLk41ea+00kTFiMw1RUH8lvUB+qeOWoOGi";
		strBase64 += "TnQhyrR92DH3OvZmMS6AX7/jt4XDGdLcEzm9TS4bCEAszXoiMODqs0ONzC1mdax/";
		strBase64 += "beHdgviWHIe+tvFBZogGPON06iyzA2udnqtqbv8jvZX6yZKeDboO9ofNpMwdSi6k";
		strBase64 += "uoHNlFH2fRgUhCzUI1Zf/o51GcnaOL845kyzEKo9MwkiY+tulJBsTAcfoLTmfdr8";
		strBase64 += "v9Cj9GoKRvEv/L2nbv3TvyESsyKywctLTvI7PiZT9Otns1ZLAoIBAQC+5Hlb7vyM";
		strBase64 += "rq+h8fbA4CMBvl/bCwgbb4Ou0ZdnWd+lQiWopWc/kLnX4TbzCKxrs6z6ii0rEzSE";
		strBase64 += "/i+561CeMwxeiub5kuOKl22lWwS9Nuxwo0zoIJYDVTnxxxjZMaDkMemA5piTUBRs";
		strBase64 += "hwiloLswC3jjY/F0oq3V3VNvwGkdprQS1CooenmgrxzKocaDwiSMdPCIHKgLQPWn";
		strBase64 += "XgUJ7NvggReZWhe6Y2Ultu0iSQfWSHIw5BxBZAteXxg64GgNStBIOTqlTvF16paA";
		strBase64 += "PDnRgxjP7x3mEIYtKHzZZ68ZrJTHeejtTsoA3+Lkc44fs7ckHLkhOjDjvXNJcYc3";
		strBase64 += "M3YSTLTFjOmpAoIBACMC43VDgm3hbijbv2vNAU2w7Bp/mEvZWkNRDQ2VaWdxrRIV";
		strBase64 += "qwpG/OOnqE/tByIklHEYP+mB3QdhUMG9ik60IGi35UL9L9tpFmtaMHTHMnE2oIBe";
		strBase64 += "EZbDNq/EzfyQYgZFLh39npp8Om/ePyBxSIJ03Q0Y0WuNvepLOgROZUcsDnx8nS95";
		strBase64 += "QHxymW2kwW1N4pCr3v+rLiptsly/aTuvGYtzKUa1rE7AjRlPlrVjanA4B9dUda7j";
		strBase64 += "ndpS7v9ngvpysAXsT13JvIEscs25Sz+EJf2SRacF5Amb1ahSKgH9FvxneHxXSgjl";
		strBase64 += "D8iornwTSrciR1MjGZKVGbJ15AaK0Rh2MKcbxhMCggEBAKtE8l25nn3afVofbfS/";
		strBase64 += "JKl71SUCUNPMyakgA2eHC1GKd59IR8qQXy2Bs3t6XLiFDL39G2l6YbSxGsEEj+Bg";
		strBase64 += "jMiGd5WUZPMDyiyB7U6W6Kxm16mtEusKnGKA75pvHJug7I/qVRpWzvxQa0GvZENH";
		strBase64 += "B798/DZZfv5TBXlOeQFRgF8kXG7U9Ctm8b4cR7APkBs1EFoO2/a84aq4gGzXRq+3";
		strBase64 += "IyVD3+YrJ6Ue6sPIIm/vsASNpYxF5q1VRLgOf0ztwtIKBsQqgBn7z+xeHMINjUWC";
		strBase64 += "lKDGUpxRpzKoBEJUxWfzyTihaDxAo6G0y99FrxuHadKWEydB4x4R7e9pDJZ+m/Ji";
		strBase64 += "64ECggEBAJAghDL5eyyzDnuYHC2CpJKN77VEtC7IZNeK5Sz/oYUIDP8viUmcs0i7";
		strBase64 += "P4kOnXtwTDuzoMp3b1kwkxCdax0WoKYjxhMz42A1Lr4jwgADb66hjiPOsZyy/nlc";
		strBase64 += "26oS9FTdtuKU4QkilBLwFnchNKbkijKKbK/AARCW1/35ljRR975OFTRmH2uboc3C";
		strBase64 += "+JrNjmMJ9SDfN7EWAW9H+ksP54h67lc34LwvAGox/LyY3XPINSHrPRC+DVLaqdek";
		strBase64 += "MlO++iwerFB+znNyfyiX/WKLkdTXEerkB2dCNhvY08dpJfUNirnHQrRXTrOoaC7k";
		strBase64 += "0xIFwGI6mUOh3ZfSGPxThMc7kzdRx9s=";
	
		CkBinData BinData;
		BinData.LoadEncoded(strBase64, "base64");
		m_PrivateKeyProduction.LoadAnyFormat(BinData, "");
	}

	ResetTokens();
	m_bRestConnected = FALSE;
	m_bDoneFirstAPICall = FALSE;
}

//$$******************************************************************

void CPebbleHelpers::CleanUp()
{
	m_Rest.Disconnect(1000);
}

//$$******************************************************************

void CPebbleHelpers::ResetTokens()
{
	m_strJWT = "";
	m_strIAT = "";
	m_nJWTExpireTime = 0;
	m_nIATExpireTime = 0;
	m_Rest.Disconnect(1000);
	m_bRestConnected = FALSE;
}

//$$******************************************************************

CString CPebbleHelpers::GetJWTAuth()
{
	CString strResult = "";
	strResult += "Bearer ";
	strResult += m_strJWT;
	return strResult;
}

//$$******************************************************************

CString CPebbleHelpers::GetIATAuth()
{
	CString strResult = "";
	strResult += "token ";
	strResult += m_strIAT;
	return strResult;
}

//$$******************************************************************

void CPebbleHelpers::ClearRest()
{
	m_Rest.ClearAllHeaders();
	m_Rest.ClearAllParts();
	m_Rest.ClearAllPathParams();
	m_Rest.ClearAllQueryParams();
	m_Rest.ClearAuth();
	m_Rest.ClearResponseBodyStream();
}

//$$******************************************************************

void CPebbleHelpers::LogRestError()
{
	COleDateTime timeNow = COleDateTime::GetCurrentTime();
	
	CString strTimeNow = "";
	strTimeNow.Format("%2.2d/%2.2d/%4.4d @ %2.2d:%2.2d:%2.2d",
		timeNow.GetDay(),
		timeNow.GetMonth(),
		timeNow.GetYear(),
		timeNow.GetHour(),
		timeNow.GetMinute(),
		timeNow.GetSecond());

	CSSFile fileLog;
	if (fileLog.Open(Filenames.GetRestErrorLogFilename(), "ab") == TRUE)
	{
		fileLog.WriteLine("");
		fileLog.WriteLine(strTimeNow);
		fileLog.WriteLine("******************************");
		fileLog.WriteLine(m_Rest.lastErrorText());
	}
}

//$$******************************************************************

CString CPebbleHelpers::GetPebbleURLRoot()
{
	if (Server.GetPebbleDemoFlag() == TRUE)
	{
		return "trac.integration.pebble.cloud";
	}
	else
	{
		return "trac.mypebble.co.uk";
	}
}

//$$******************************************************************

bool CPebbleHelpers::UpdateJWT()
{
	if (m_strJWT != "")
	{
		CkJwt jwtTest;
		int nTestExpireTime = jwtTest.GenNumericDate(0) + 120;

		if (m_nJWTExpireTime > nTestExpireTime)
		{
			return TRUE;
		}
	}

	m_strJWT = "";
	m_nJWTExpireTime = 0;

	CkJwt jwt;

	// Build the JOSE header
	CkJsonObject jose;
	jose.AppendString("alg", "RS256");
	jose.AppendString("typ", "JWT");

	// Now build the JWT claims (also known as the payload)
	CkJsonObject claims;
	claims.AppendString("iss", "smartpay");

	{
		CString strAud = "https://";
		strAud += GetPebbleURLRoot();
		claims.AppendString("aud", strAud);
	}

	// Set the timestamp of when the JWT was created to now.
	int curDateTime = jwt.GenNumericDate(0);
	claims.AddIntAt(-1, "iat", curDateTime);

	// Set the epxiry time for the JWT
	m_nJWTExpireTime = curDateTime + JSONWT_TTL;
	claims.AddIntAt(-1, "exp", m_nJWTExpireTime);

	// Produce the smallest possible JWT:
	jwt.put_AutoCompact(true);

	m_strJWT = jwt.createJwtPk(jose.emit(), claims.emit(),
		Server.GetPebbleDemoFlag() ? m_PrivateKeyDemo : m_PrivateKeyProduction);
	
	return TRUE;
}

//$$******************************************************************

bool CPebbleHelpers::UpdateRestConnect(CString& strError)
{
	strError = "";

	if (TRUE == m_bRestConnected)
	{
		return TRUE;
	}

	bool bTls = true;
	int port = 443;
	bool bAutoReconnect = true;
	CString strURLRoot = GetPebbleURLRoot();

	m_bRestConnected = m_Rest.Connect(strURLRoot, port, bTls, bAutoReconnect);
	
	if (TRUE == m_bRestConnected)
	{
		ClearRest();
	}
	else
	{
		LogRestError();
		strError = "Failed to establish REST connection to Pebble API";
	}

	return m_bRestConnected;
}

//$$******************************************************************

bool CPebbleHelpers::UpdateIAT(CSQLRepositoryPebbleConfig& PebbleConfig, CString& strError, CWorkingDlg* pWorkingDlg )
{
	strError = "";

	if (m_strIAT != "")
	{
		CkJwt jwtTest;
		int nTestExpireTime = jwtTest.GenNumericDate(0) + 120;

		if (m_nIATExpireTime > nTestExpireTime)
		{
			return TRUE;
		}
	}

	if (pWorkingDlg != NULL)
	{
		pWorkingDlg->SetCaption2("Acquiring Access Token");
	}

	m_strIAT = "";
	m_nIATExpireTime = 0;

	if (UpdateJWT() == FALSE)
	{
		strError = "Unable to create JSON token for Pebble API";
		return FALSE;
	}
	
	if (UpdateRestConnect(strError) == FALSE)
	{
		return FALSE;
	}

	ClearRest();

	m_Rest.AddHeader("accept", "application/json");
	m_Rest.AddHeader("authorization", GetJWTAuth());

	CString strURL = "";
	strURL.Format("/api/v1/app/installations/%s/access-tokens",
		(const char*)PebbleConfig.m_Row.GetActiveInstallationID());

	CkStringBuilder sbResponseBody;
	bool success = m_Rest.FullRequestNoBodySb("POST", strURL, sbResponseBody);
	if (success != true) 
	{
		LogRestError();
		strError = "Pebble API Get Access Token Fail";
		ResetTokens();
		return FALSE;
	}

	if (m_Rest.get_ResponseStatusCode() >= 400)
	{
		strError.Format("Pebble API Get Access Token Error %d", m_Rest.get_ResponseStatusCode());
		return FALSE;
	}

	CkJsonObject jsonResponse;
	jsonResponse.LoadSb(sbResponseBody);

	CkString result;
	jsonResponse.StringOf("token", result);
	m_strIAT = result.getStringUtf8();
	m_nIATExpireTime = jsonResponse.IntOf("expiresAt");

	return TRUE;
}

//$$******************************************************************

void CPebbleHelpers::Diagnostic(CkJsonObject* pObject, bool bReceive, CString strCaption)
{
	if (Server.GetPebbleDiagnosticType() != 0)
	{
		if (pObject != NULL)
		{
			pObject->put_EmitCompact(FALSE);
			CString str = pObject->emit();
			Diagnostic(str, bReceive, strCaption);
		}
	}
}

//$$******************************************************************

void CPebbleHelpers::Diagnostic(CString strMsg, bool bReceive, CString strCaption)
{
	if (Server.GetPebbleDiagnosticType() != 0)
	{
		COleDateTime timeNow = COleDateTime::GetCurrentTime();

		CString strLogCaption = "";
		strLogCaption.Format("%2.2d/%2.2d/%4.4d @ %2.2d:%2.2d:%2.2d  %s  %s",
			timeNow.GetDay(),
			timeNow.GetMonth(),
			timeNow.GetYear(),
			timeNow.GetHour(),
			timeNow.GetMinute(),
			timeNow.GetSecond(),
			(const char*)strCaption,
			bReceive ? " (RECEIVE)" : " (SEND)");

		if ((Server.GetPebbleDiagnosticType() == 2) && (Server.GetPebbleDiagnosticMessageBoxFlag() == TRUE))
		{
			if (Prompter.YesNo(strMsg, strCaption, TRUE, FALSE) != IDYES)
			{
				Server.SetPebbleDiagnosticMessageBoxFlag(FALSE);
			}
		}

		CSSFile fileDump;
		if (fileDump.Open(Filenames.GetPebbleJSONDumpFilename(), "ab") == TRUE)
		{
			fileDump.WriteLine(strLogCaption);
			fileDump.WriteLine("************************************************************");
			fileDump.WriteLine(strMsg);
			fileDump.WriteLine("");
		}
	}
}

//$$******************************************************************

bool CPebbleHelpers::LoadPebblePayments()
{
	return m_RowSetPebblePayment.LoadPebblePayments();
}

//$$******************************************************************

bool CPebbleHelpers::IsValidSPOSPaymentType(int nType, bool bTopUp)
{
	bool bResult = FALSE;

	if (CSQLRowPebblePaymentType::IsValidSPOSPaymentType(nType, bTopUp) == TRUE)
	{
		for (int n = 0; n < m_RowSetPebblePayment.GetSize(); n++)
		{
			CSQLRowPebblePaymentType RowPayment;
			m_RowSetPebblePayment.GetRow(n, RowPayment);

			if (RowPayment.GetSPOSPaymentType() == nType)
			{
				bResult = TRUE;
				break;
			}
		}
	}

	return bResult;
}

//$$******************************************************************
