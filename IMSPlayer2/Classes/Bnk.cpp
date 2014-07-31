#include "Bnk.h"
#include "IMSManager.h"

Bnk::Bnk () {
	m_bnk = 0;
}

Bnk::~Bnk ()
{
	free(m_bnk->nameRecord);
	free(m_bnk->instRecord);
	free(m_bnk->instRecord32);
	free(m_bnk->raw);
	free(m_bnk);
}

MYADLIB_BNK *Bnk::GetBnk()
{
	return m_bnk;
}

int Bnk::GetTotalInst()
{
	return m_bnk->header.totalEntry;
}

bool Bnk::Open(std::string file)
{
	char signature[6+1];
	int i;

	if(m_bnk)
	{
		delete m_bnk->raw;
		delete m_bnk->instRecord32;
		delete m_bnk->instRecord;
		delete m_bnk->nameRecord;
		delete m_bnk;		
	}

	m_bnk = (MYADLIB_BNK *)malloc(sizeof(MYADLIB_BNK));
	if (m_bnk == NULL) 
		return false;

	m_fileName = file;

	//char path[1024];
	//strcpy( path, (const char*)file.mb_str(wxConvUTF8) ); 

	char* file_contents = NULL;
	
	if (IMSManager::read_text(m_fileName.c_str(), &file_contents, m_fileSize) != true)
	{
		delete  m_bnk;
		m_bnk = NULL;
		return 0;
	}

	if (m_fileSize > 1024*1024) 
	{
		return false;
	}

	m_bnk->raw = (char *)file_contents;
	if (m_bnk->raw == NULL) 
	{
		return false;
	}
	
	// header
	memcpy(&m_bnk->header, m_bnk->raw, sizeof(MYADLIB_BNK_HEADER));
    strncpy(signature, m_bnk->header.signature, 6);
	signature[6] = 0;

	if (strcmp(signature, "ADLIB-") != 0) 
		{
		if (strcmp(signature, "adlib-") != 0) 
		{
			return false;
		}
		}
		
	// name record
	m_bnk->nameRecordSize = 
		sizeof(MYADLIB_BNK_NAME_RECORD) * m_bnk->header.totalEntry;
	m_bnk->nameRecord = 
		(MYADLIB_BNK_NAME_RECORD *)malloc(m_bnk->nameRecordSize);
	if (m_bnk->nameRecord == NULL) 
		return false;

	memcpy(m_bnk->nameRecord, 
			m_bnk->raw + m_bnk->header.recSeekPos, 
			m_bnk->nameRecordSize);

	// instrument record
	m_bnk->instRecordSize = 
		sizeof(MYADLIB_BNK_INST_RECORD) * m_bnk->header.totalEntry;
	m_bnk->instRecord = 
		(MYADLIB_BNK_INST_RECORD *)malloc(m_bnk->instRecordSize);
	if (m_bnk->instRecord == NULL) 
		return false;

	memcpy(m_bnk->instRecord, m_bnk->raw + m_bnk->header.dataSeekPos, 
			m_bnk->instRecordSize);

	// instrument record 32
	m_bnk->instRecordSize32 = sizeof(MYADLIB_BNK_INST_RECORD32) * 
		m_bnk->header.totalEntry;
	m_bnk->instRecord32 = 
		(MYADLIB_BNK_INST_RECORD32 *)malloc(m_bnk->instRecordSize32);
	if (m_bnk->instRecord32 == NULL) 
		return false;

	for (i = 0; i < m_bnk->header.totalEntry; i++) 
	{
		//wxString instName(m_bnk->nameRecord[i].name, wxConvUTF8);
		//wxMessageBox(instName);

		m_bnk->instRecord32[i].instType			   
			= m_bnk->instRecord[i].instType;
		m_bnk->instRecord32[i].voiceNum			   
			= m_bnk->instRecord[i].voiceNum;
		m_bnk->instRecord32[i].op1.keyScaleLevel    
			= m_bnk->instRecord[i].op1.keyScaleLevel;
		m_bnk->instRecord32[i].op1.freqMultiplier   
			= m_bnk->instRecord[i].op1.freqMultiplier;
		m_bnk->instRecord32[i].op1.feedback         
			= m_bnk->instRecord[i].op1.feedback;
		m_bnk->instRecord32[i].op1.attackRate       
			= m_bnk->instRecord[i].op1.attackRate;
		m_bnk->instRecord32[i].op1.sustainLevel     
			= m_bnk->instRecord[i].op1.sustainLevel;
		m_bnk->instRecord32[i].op1.sustainSound     
			= m_bnk->instRecord[i].op1.sustainSound;
		m_bnk->instRecord32[i].op1.decayRate        
			= m_bnk->instRecord[i].op1.decayRate;
		m_bnk->instRecord32[i].op1.releaseRate      
			= m_bnk->instRecord[i].op1.releaseRate;
		m_bnk->instRecord32[i].op1.outputLevel      
			= m_bnk->instRecord[i].op1.outputLevel;
		m_bnk->instRecord32[i].op1.amplitudeVibrato 
			= m_bnk->instRecord[i].op1.amplitudeVibrato;
		m_bnk->instRecord32[i].op1.freqVibrato      
			= m_bnk->instRecord[i].op1.freqVibrato;
		m_bnk->instRecord32[i].op1.envelopeScale    
			= m_bnk->instRecord[i].op1.envelopeScale;
		m_bnk->instRecord32[i].op1.fmType           
			= m_bnk->instRecord[i].op1.fmType;
		m_bnk->instRecord32[i].op2.keyScaleLevel    
			= m_bnk->instRecord[i].op2.keyScaleLevel;
		m_bnk->instRecord32[i].op2.freqMultiplier   
			= m_bnk->instRecord[i].op2.freqMultiplier;
		m_bnk->instRecord32[i].op2.feedback         
			= m_bnk->instRecord[i].op2.feedback;
		m_bnk->instRecord32[i].op2.attackRate       
			= m_bnk->instRecord[i].op2.attackRate;
		m_bnk->instRecord32[i].op2.sustainLevel     
			= m_bnk->instRecord[i].op2.sustainLevel;
		m_bnk->instRecord32[i].op2.sustainSound     
			= m_bnk->instRecord[i].op2.sustainSound;
		m_bnk->instRecord32[i].op2.decayRate        
			= m_bnk->instRecord[i].op2.decayRate;
		m_bnk->instRecord32[i].op2.releaseRate      
			= m_bnk->instRecord[i].op2.releaseRate;
		m_bnk->instRecord32[i].op2.outputLevel      
			= m_bnk->instRecord[i].op2.outputLevel;
		m_bnk->instRecord32[i].op2.amplitudeVibrato 
			= m_bnk->instRecord[i].op2.amplitudeVibrato;
		m_bnk->instRecord32[i].op2.freqVibrato      
			= m_bnk->instRecord[i].op2.freqVibrato;
		m_bnk->instRecord32[i].op2.envelopeScale    
			= m_bnk->instRecord[i].op2.envelopeScale;
		m_bnk->instRecord32[i].op2.fmType           
			= m_bnk->instRecord[i].op2.fmType;
		m_bnk->instRecord32[i].op1WaveForm		   
			= m_bnk->instRecord[i].op1WaveForm;
		m_bnk->instRecord32[i].op2WaveForm		   
			= m_bnk->instRecord[i].op2WaveForm;
	}

	return true;
}
